#include "uart_actuator.h"
#include "board_config.h"
#include "debug_print.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void UART_ACTUA_Init(void){
    uart_config_t uart_config = {
        .baud_rate = UART_ACTUA_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    uart_param_config(UART_ACTUA_NUM, &uart_config);
    uart_set_pin(UART_ACTUA_NUM, UART_ACTUA_TXD, UART_ACTUA_RXD,
                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // 使用IDF官方驱动自带中断与缓冲区
    uart_driver_install(UART_ACTUA_NUM, RING_BUFFER_SIZE, 0, 0, NULL, 0);

    Debug_Print("UART执行器初始化完成\r\n");
}

void UART_ACTUA_SendData(uint8_t *data, size_t length){
    uart_write_bytes(UART_ACTUA_NUM, (const char *)data, length);
}

// 接口原型完全不变，内部直接读驱动缓冲区
int UART_ACTUA_RecvData(uint8_t *data, size_t length, uint32_t timeout_ms){
    return uart_read_bytes(UART_ACTUA_NUM, data, length, pdMS_TO_TICKS(timeout_ms));
}

int UART_ACTUA_GetRxBufferSize(void){
    size_t len = 0;
    uart_get_buffered_data_len(UART_ACTUA_NUM, &len);
    return (int)len;
}

void UART_ACTUA_ClearRxBuffer(void){
    uart_flush_input(UART_ACTUA_NUM);
}
