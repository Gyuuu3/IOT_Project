#ifndef __UART_ACTUATOR_H__
#define __UART_ACTUATOR_H__
#include "driver/uart.h"

void UART_ACTUA_Init(void);
void UART_ACTUA_SendData(uint8_t *data, size_t length);
int UART_ACTUA_RecvData(uint8_t *data, size_t length, uint32_t timeout_ms);
int UART_ACTUA_GetRxBufferSize(void);
void UART_ACTUA_ClearRxBuffer(void);
#endif
