#include "ext_uart_esp.h"
#include "board_config.h"
#include "protocol_frame.h"
#include "usart.h"
#include "string.h"

extern uint8_t uart2_rx_buf[];
extern volatile uint16_t uart2_rx_len;
extern volatile uint8_t uart2_frame_ready;

void ESP_UART_Send(uint8_t* buf,uint16_t len){
	HAL_UART_Transmit(&ESP_UART_HANDLE,buf,len,100);
}

void ESP_UART_SendFrame(uint8_t device_type,uint8_t device_id,uint8_t cmd,uint8_t data_len,uint8_t* data){
	MsgData_t msg ={0};
	Protocol_PackFrame(device_type,device_id,cmd,data_len,data,&msg);
	ESP_UART_Send(msg.data,msg.length);

}

uint16_t ESP_UART_GetRxLength(void){
	return uart2_rx_len;
}

uint8_t* ESP_UART_GetRxBuffer(void){
	return uart2_rx_buf;
}

void ESP_UART_ClearRxFlag(void){
	uart2_frame_ready=0;
}

uint8_t ESP_UART_IsFrameReady(void){
	return uart2_frame_ready;
}







