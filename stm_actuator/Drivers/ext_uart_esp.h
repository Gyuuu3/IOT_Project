#ifndef __EXT_UART_ESP_H__
#define __EXT_UART_ESP_H__

#include "stdint.h"
#include "ring_buffer.h"

void ESP_UART_Send(uint8_t* buf,uint16_t len);
void ESP_UART_SendFrame(uint8_t device_type,uint8_t device_id,uint8_t cmd,uint8_t data_len,uint8_t* data);

uint16_t ESP_UART_GetRxLength(void);
uint8_t* ESP_UART_GetRxBuffer(void);
void ESP_UART_ClearRxFlag(void);
uint8_t ESP_UART_IsFrameReady(void);

#endif
