#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__
#include "stdint.h"

typedef struct{
	uint8_t *buf;	//缓冲区数组
	uint16_t size;	//缓冲区大小
	uint16_t wptr;	//写指针
	uint16_t rptr;	//读指针
	
} RingBuffer;

void RingBuffer_Init(RingBuffer* rb,uint8_t* buf,uint16_t size);
uint8_t RingBuffer_Write(RingBuffer* rb,uint8_t data);
uint8_t RingBuffer_Read(RingBuffer* rb,uint8_t* data);

#endif
