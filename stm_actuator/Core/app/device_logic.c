#include "device_logic.h"
#include "usart.h"
#include "ext_gpio_led.h"
#include "protocol_frame.h"
#include "board_config.h"
/*
void Device_ESP_Cmd(uint8_t cmd){
	switch(cmd){
		case CMD_LED_ON:
			LED_ON();
			printf("ESP命令:[开灯]\r\n");
			break;
		case CMD_LED_OFF:
			LED_OFF();
			printf("ESP命令:[关灯]\r\n");
			break;
		case CMD_MOTOR_ON:
			
			printf("ESP命令:[启动电机]\r\n");
			break;
		case CMD_MOTOR_OFF:
			
			printf("ESP命令:[关闭电机]\r\n");
			break;
		default:
			printf("未知指令:[%d]\r\n",cmd);
	}

}
*/

/*
uint8_t Device_ParseESPCmd(RingBuffer* rb,uint8_t* out_cmd){
	//uint8_t temp,find_head=0; 记录包头 
	// 缓冲区有数据可读时，读出数据存入temp 
	while(RingBuffer_Read(rb,&temp)){
		if(temp==ESP_FRAME_HEAD){
			find_head=1;
			continue;
		}
		if(find_head){
			*out_cmd=temp;
			if(!RingBuffer_Read(rb,&temp))	return 0;
			if(temp==ESP_FRAME_TAIL)		return 1;
		}
	}
	
	return 0;					
	
}
*/


