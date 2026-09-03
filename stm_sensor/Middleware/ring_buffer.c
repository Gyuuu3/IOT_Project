#include "ring_buffer.h"
/*
typedef struct{
	uint8_t *buf;	//缓冲区数组
	uint16_t size;	//缓冲区大小
	uint16_t wptr;	//写指针
	uint16_t rptr;	//读指针
	
} RingBuffer;
*/
void RingBuffer_Init(RingBuffer* rb,uint8_t* buf,uint16_t size){
	
	rb->buf=buf;
	rb->size=size;
	rb->wptr=0;
	rb->rptr=0;

}

uint8_t RingBuffer_Write(RingBuffer* rb,uint8_t data){
	
	if(((rb->wptr+1)%rb->size)==rb->rptr)	return 0;

	rb->buf[rb->wptr]=data;
	rb->wptr=(rb->wptr+1)%rb->size;
	return 1;
}

uint8_t RingBuffer_Read(RingBuffer* rb,uint8_t* data){
	if(rb->rptr==rb->wptr)	return 0;
	
	*data=rb->buf[rb->rptr];
	rb->rptr=(rb->rptr+1)%rb->size;
	return 1;

}











