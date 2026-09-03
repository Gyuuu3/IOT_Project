#ifndef __PROTOCOL_FRAME_H__
#define __PROTOCOL_FRAME_H__
#include "stdint.h"
#include "board_config.h"
#include "stdbool.h"

typedef struct {
	uint8_t head;
	uint8_t device_type;
	uint8_t device_id;
	uint8_t cmd;
	uint8_t data_len;
	uint8_t data[64];
	uint8_t tail;

} ProtocolFrame_t;

typedef struct {
	uint8_t data[FRAME_MAX_SIZE];
	uint16_t length;
} MsgData_t;

void Protocol_PackFrame(uint8_t device_type,uint8_t device_id,
				uint8_t cmd,uint8_t data_len,uint8_t* data,MsgData_t* out_msg );
bool Protocol_UnpackFrame(uint8_t* data,uint16_t length,ProtocolFrame_t* out_frame);

#endif
