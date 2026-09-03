#ifndef __RINGBUFFER_H__
#define __RING_BUFFER_H__
#include "stdint.h"
#include "board_config.h"

typedef struct{
    uint8_t* buf;
    uint16_t size;
    uint16_t wptr;
    uint16_t rptr;
} RingBuffer_t;



void RingBuffer_Init(RingBuffer_t* rb,uint8_t* buf,uint16_t size);
uint8_t RingBuffer_Write(RingBuffer_t *rb,uint8_t data);
uint8_t RingBuffer_Read(RingBuffer_t *rb,uint8_t* data);

#endif
