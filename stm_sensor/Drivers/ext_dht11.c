#include "ext_dht11.h"
#include "FreeRTOS.h"
#include "task.h"
#include "board_config.h"
#include "gpio.h"

#define DHT11_MIN_INTERVAL pdMS_TO_TICKS(2000)

// 供 DHT11_GetTemp / DHT11_GetHumi 共享的静态数据（避免局部变量last_read_time为垃圾值）
static DHT11_Data_t g_dht11_data = {0};

/*
 * 微秒级忙等延时（基于 SysTick 当前值递减寄存器）
 * 说明：SysTick->VAL 是 24 位递减计数器，(start - current) & 0x00FFFFFF
 *       可正确处理计数器从 0 重载到 LOAD 的溢出回绕。
 */
static void DHT11_Delay_us(uint32_t us)
{
    uint32_t ticks = us * (SystemCoreClock / 1000000UL);
    uint32_t start = SysTick->VAL;
    uint32_t current;
    do {
        current = SysTick->VAL;
        uint32_t elapsed = (start - current) & 0x00FFFFFF;
        if (elapsed >= ticks) break;
    } while (1);
}

/*
 * 配置 DHT11 引脚为推挽输出模式
 * 【修复】移除了原来的 static bool init 只初始化一次的限制。
 * 原因：每次读取都需要在 输出 <-> 输入 之间切换，
 *       如果 SetOutput 只生效一次，读取结束后引脚会停在输入模式，
 *       下一次调用 HAL_GPIO_WritePin(RESET) 无法拉低引脚，DHT11 收不到起始信号。
 */
static void DHT11_SetOutput(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin   = DHT11_PIN;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

/*
 * 配置 DHT11 引脚为上拉输入模式（读取 DHT11 数据时使用）
 */
static void DHT11_SetInput(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pin   = DHT11_PIN;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

/*
 * 从 DHT11 读取一个字节（8 bit）
 * DHT11 位编码：
 *   每 bit 以 50us 低电平开始，随后高电平持续：
 *     - 26~28us 表示 bit 0
 *     - 70us    表示 bit 1
 *   策略：等过低电平后延时 40us，此时若仍为高电平则是 bit 1，否则是 bit 0。
 */
static uint8_t DHT11_ReadByte(void)
{
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        // 等待 bit 起始的 50us 低电平结束（超时保护）
        volatile uint32_t timeout = 100UL * (SystemCoreClock / 1000000UL);
        while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_RESET)
        {
            if (timeout-- == 0) return 0;
        }

        // 延时 40us 后采样：高电平=1，低电平=0
        DHT11_Delay_us(40);
        if (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET)
        {
            byte |= (1 << (7 - i));
        }

        // 若是 bit 1，高电平还会持续约 30us，等待其结束（超时保护）
        timeout = 100UL * (SystemCoreClock / 1000000UL);
        while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET)
        {
            if (timeout-- == 0) return 0;
        }
    }
    return byte;
}

/*
 * 读取一次 DHT11 温湿度数据
 * 返回值：true=读取成功且校验通过，false=冷却中/超时/校验失败
 *
 * 【改进】18ms 起始信号拉低移到临界区外执行，避免 taskENTER_CRITICAL 后
 *         关中断长达 18ms 影响系统实时性（可能导致串口丢包、任务调度卡死）。
 *         仅在拉高→切换输入→读取40bit 的关键时序段进入临界区。
 */
bool DHT11_Read(DHT11_Data_t *data)
{
    uint8_t humi_high, humi_low, temp_high, temp_low, checksum;
    bool result = false;
    uint32_t now_tick = xTaskGetTickCount();

    if (data == NULL) return false;

    // 2 秒冷却保护：DHT11 采样间隔建议 ≥ 2 秒，否则读数可能异常
    if ((now_tick - data->last_read_time) < DHT11_MIN_INTERVAL)
    {
        data->valid = false;
        return false;
    }

    // ====== 重试机制：最多尝试 3 次 ======
    for (uint8_t retry = 0; retry < 3; retry++)
    {
        result = false;

        // ---------------------------------------------------------------
        // 阶段1：起始信号（临界区外，避免长时间关中断）
        //   主机拉低总线 ≥ 18ms，通知 DHT11 准备发送数据
        // ---------------------------------------------------------------
        DHT11_SetOutput();
        HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);
        DHT11_Delay_us(18000);   // 拉低 18ms

        // ---------------------------------------------------------------
        // 阶段2：关键时序段（进入临界区，防止中断打断微秒级时序）
        //   拉高 20~40us → 切换输入 → 等待 DHT11 响应 → 读取 40bit
        // ---------------------------------------------------------------
        taskENTER_CRITICAL();

        HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
        DHT11_Delay_us(30);      // 拉高 30us 后释放总线
        DHT11_SetInput();

        // 等待 DHT11 响应：80us 低电平
        volatile uint32_t timeout = 200UL * (SystemCoreClock / 1000000UL);
        while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_RESET)
        {
            if (timeout-- == 0) goto exit_critical;
        }

        // 等待 DHT11 响应：80us 高电平
        timeout = 200UL * (SystemCoreClock / 1000000UL);
        while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET)
        {
            if (timeout-- == 0) goto exit_critical;
        }

        // 连续读取 5 个字节：湿度整数、湿度小数、温度整数、温度小数、校验和
        humi_high = DHT11_ReadByte();
        humi_low  = DHT11_ReadByte();
        temp_high = DHT11_ReadByte();
        temp_low  = DHT11_ReadByte();
        checksum  = DHT11_ReadByte();

        // 校验：前 4 字节之和 == 第 5 字节（校验和）
        if ((humi_high + humi_low + temp_high + temp_low) == checksum)
        {
            data->temperature = (float)temp_high + (float)temp_low / 10.0f;
            data->humidity    = (float)humi_high + (float)humi_low / 10.0f;
            result = true;
        }

exit_critical:
        // 读取结束：恢复为输出模式并拉高总线，为下一次读取做好准备
        DHT11_SetOutput();
        HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
        taskEXIT_CRITICAL();

        if (result) break;   // 成功则退出重试

        // 失败则等待 100ms 再重试（临界区外，允许中断和任务调度）
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    if (result)
    {
        data->valid = true;
        data->last_read_time = now_tick;
    }
    else
    {
        data->valid = false;
    }

    return result;
}

/*
 * 便捷接口：读取温度（失败返回 -999.0f）
 * 【修复】使用文件级静态变量 g_dht11_data，与 GetHumi 共享状态，
 *         避免原代码局部变量 last_read_time 为未初始化垃圾值导致冷却保护失效。
 */
float DHT11_GetTemp(void)
{
    if (DHT11_Read(&g_dht11_data)) return g_dht11_data.temperature;
    return -999.0f;
}

/*
 * 便捷接口：读取湿度（失败返回 -999.0f）
 * 与 DHT11_GetTemp 共享同一个静态数据结构，不会重复触发读取。
 */
float DHT11_GetHumi(void)
{
    if (DHT11_Read(&g_dht11_data)) return g_dht11_data.humidity;
    return -999.0f;
}
