#include "protocol_frame.h"
#include "board_config.h"
#include "string.h"

void Protocol_PackFrame(uint8_t device_type,uint8_t device_id,uint8_t cmd,
					uint8_t data_len,uint8_t* data,MsgData_t* out_msg)
{
	uint8_t idx=0;
	if(data_len>64)	data_len=64;
	
	out_msg->data[idx++]=FRAME_HEAD;
	out_msg->data[idx++]=device_type;
	out_msg->data[idx++]=device_id;
	out_msg->data[idx++]=cmd;
	out_msg->data[idx++]=data_len;
	
	if(data!=NULL&&data_len>0){
		memcpy(&out_msg->data[idx],data,data_len);
		idx+=data_len;
	}
	
	out_msg->data[idx++]=FRAME_TAIL;
	out_msg->length=idx;

}

bool Protocol_UnpackFrame(uint8_t* data,uint16_t length,ProtocolFrame_t* out_frame){
	if(length<6)	return false;

	uint8_t idx=0;
	while(idx<length-5){
		if(data[idx]==FRAME_HEAD){
			out_frame->head=data[idx++];
			out_frame->device_type=data[idx++];
			out_frame->device_id=data[idx++];
			out_frame->cmd=data[idx++];
			out_frame->data_len=data[idx++];
			
			if(out_frame->data_len>64)	return false;
			uint16_t data_end=idx+out_frame->data_len;
			if(data_end>length)		return false;
			if(data[data_end]!=FRAME_TAIL)	return false;
			
			if(out_frame->data_len>0){
				memcpy(out_frame->data,&data[idx],out_frame->data_len);
			}
			
			out_frame->tail=data[data_end];
			return true;
		}
		idx++;
	}
	return false;
}



