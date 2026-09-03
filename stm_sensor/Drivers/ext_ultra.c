#include "ext_ultra.h"
#include "FreeRTOS.h"
#include "task.h"
#include "board_config.h"
#include "tim.h"      

#define SOUND_SPEED_CM_US 0.034f

// 【微秒延时函数】用于触发脉冲，保持原样
static void Ultra_Delay_us(uint32_t us)
{
    volatile uint32_t ticks = us * (SystemCoreClock / 1000000UL);
    while(ticks--) __NOP();
}

// 【获取当前微秒时间】直接读取 TIM4 计数器（1us 分辨率，16位，最大65ms）
static uint32_t Ultra_GetTimeUs(void)
{
    return (uint32_t)__HAL_TIM_GET_COUNTER(&htim4);
}

bool Ultra_Read(Ultra_Data_t *data)
{
    uint32_t start_us, end_us, duration_us;
    uint32_t tick_start, tick_end;

    if (data == NULL) return false;

    tick_start = xTaskGetTickCount();
    if ((tick_start - data->last_read_time) < pdMS_TO_TICKS(200)) {
        data->valid = false;
        return false;
    }

    // ===== 1. 发送 10us 触发脉冲 =====
    taskENTER_CRITICAL();
    HAL_GPIO_WritePin(ULTRA_TRIG_PORT, ULTRA_TRIG_PIN, GPIO_PIN_SET);
    Ultra_Delay_us(10);
    HAL_GPIO_WritePin(ULTRA_TRIG_PORT, ULTRA_TRIG_PIN, GPIO_PIN_RESET);
    taskEXIT_CRITICAL();

    // ===== 2. 等待 ECHO 上升沿（超时 50ms） =====
    tick_end = xTaskGetTickCount() + pdMS_TO_TICKS(50);
    while (HAL_GPIO_ReadPin(ULTRA_ECHO_PORT, ULTRA_ECHO_PIN) == GPIO_PIN_RESET) {
        if (xTaskGetTickCount() > tick_end) {
            data->valid = false;
            return false;
        }
        taskYIELD();
    }
    // 【记录开始时间（微秒）】
    start_us = Ultra_GetTimeUs();

    // ===== 3. 等待 ECHO 下降沿（超时 50ms） =====
    tick_end = xTaskGetTickCount() + pdMS_TO_TICKS(50);
    while (HAL_GPIO_ReadPin(ULTRA_ECHO_PORT, ULTRA_ECHO_PIN) == GPIO_PIN_SET) {
        if (xTaskGetTickCount() > tick_end) {
            data->valid = false;
            return false;
        }
        taskYIELD();
    }
    // 【记录结束时间（微秒）】
    end_us = Ultra_GetTimeUs();

    // ===== 4. 计算持续时间（处理 TIM4 16位溢出） =====
    if (end_us >= start_us) {
        duration_us = end_us - start_us;
    } else {
        // 如果发生了溢出（Counter 从 65535 回到 0），加上周期长度
        duration_us = (65535 - start_us) + end_us + 1;
    }

    // ===== 5. 计算距离 =====
    // 有效范围：100us ~ 30000us（对应约 1.7cm ~ 5.1m）
    if (duration_us > 100 && duration_us < 30000) {
        data->distance_cm = (float)duration_us * SOUND_SPEED_CM_US / 2.0f;
        data->valid = true;
        data->last_read_time = tick_start;
        return true;
    }

    data->valid = false;
    data->distance_cm = -1.0f;
    return false;
}

float Ultra_GetDistance(void)
{
    Ultra_Data_t data;
    if (Ultra_Read(&data)) return data.distance_cm;
    return -1.0f;
}
