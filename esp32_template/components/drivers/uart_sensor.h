#ifndef __UART_SENSOR_H__
#define __UART_SENSOR_H__
#include "driver/uart.h"
#include "stdint.h"

void UART_SENSOR_Init(void);
void UART_SENSOR_SendData(uint8_t *data, size_t length);
int UART_SENSOR_RecvData(uint8_t *data, size_t length, uint32_t timeout_ms);
int UART_SENSOR_GetRxBufferSize(void);
void UART_SENSOR_ClearRxBuffer(void);
#endif