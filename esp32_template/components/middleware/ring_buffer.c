#include "ring_buffer.h"
/* 暂时没用到，留着先 */

void RingBuffer_Init(RingBuffer_t* rb,uint8_t* buf,uint16_t size){
    rb->buf=buf;
    rb->size=size;
    rb->rptr=0;
    rb->wptr=0;
}

uint8_t RingBuffer_Write(RingBuffer_t *rb,uint8_t data){
    if((rb->wptr+1)%rb->size==rb->rptr) return 0;
    
    rb->buf[rb->wptr]=data;
    rb->wptr=(rb->wptr+1)%rb->size;
    return 1;
}

uint8_t RingBuffer_Read(RingBuffer_t *rb,uint8_t* data){
    if(rb->rptr%rb->size==rb->wptr) return 0;

    *data=rb->buf[rb->rptr];
    rb->rptr=(rb->rptr+1)%rb->size;
    return 1;
}

