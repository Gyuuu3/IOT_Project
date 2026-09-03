#include "uart_sensor.h"
#include "board_config.h"
#include "debug_print.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*
 * 【修复】移除了自定义中断处理 + 环形缓冲区 + 信号量的复杂结构。
 *
 * 原因：原代码 esp_intr_alloc(UART_SENSOR_NUM, ...) 传的是 UART_NUM_1=1，
 *       但 esp_intr_alloc 第一个参数要求中断源编号，UART1 的正确中断源是
 *       ETS_UART1_INTR_SOURCE=6。传1导致自定义中断注册到了 WiFi MAC NMI 中断，
 *       UART1 收到数据时自定义中断永远不触发，数据留在 uart_driver_install 的
 *       内部缓冲区里，任务从用户环形缓冲区读不到任何数据。
 *
 * 修复：uart_driver_install 已经安装了正确的默认中断处理，数据会自动存入
 *       驱动内部环形缓冲区。任务直接调用 uart_read_bytes 读取即可，
 *       不需要自己再实现中断+环形缓冲区。
 */

void UART_SENSOR_Init(void){
    uart_config_t uart_config = {
        .baud_rate = UART_SENSOR_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    uart_param_config(UART_SENSOR_NUM, &uart_config);
    uart_set_pin(UART_SENSOR_NUM, UART_SENSOR_TXD, UART_SENSOR_RXD,
                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // 安装驱动：接收缓冲区 RING_BUFFER_SIZE(4096) 字节，发送缓冲区0（阻塞发送）
    // 第5个参数 intr_alloc_flags=0，第6个参数不获取中断句柄
    uart_driver_install(UART_SENSOR_NUM, RING_BUFFER_SIZE, 0, 0, NULL, 0);

    Debug_Print("UART传感器初始化完成\r\n");
}

void UART_SENSOR_SendData(uint8_t *data, size_t length){
    uart_write_bytes(UART_SENSOR_NUM, (const char *)data, length);
}

/*
 * 从驱动内部缓冲区读取数据
 * @param data       接收缓冲区
 * @param length     期望读取的最大字节数
 * @param timeout_ms 超时时间（毫秒），0表示非阻塞
 * @return 实际读取的字节数
 */
int UART_SENSOR_RecvData(uint8_t *data, size_t length, uint32_t timeout_ms){
    return uart_read_bytes(UART_SENSOR_NUM, data, length, pdMS_TO_TICKS(timeout_ms));
}

int UART_SENSOR_GetRxBufferSize(void){
    size_t size = 0;
    uart_get_buffered_data_len(UART_SENSOR_NUM, &size);
    return (int)size;
}

void UART_SENSOR_ClearRxBuffer(void){
    uart_flush_input(UART_SENSOR_NUM);
}
