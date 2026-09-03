#ifndef __DEVICE_LOGIC_H__
#define __DEVICE_LOGIC_H__
#include "stdint.h"
#include "ring_buffer.h"

void Device_ESP_Cmd(uint8_t cmd);
uint8_t Device_ParseESPCmd(RingBuffer* rb,uint8_t* out_cmd);
#endif
