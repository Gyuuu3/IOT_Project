#include "task_sensor.h"
#include "ext_dht11.h"
#include "ext_ultra.h"
#include "ext_adc_dma.h"
#include "protocol_frame.h"
#include "board_config.h"
#include "task_config.h"
#include "ext_dht11.h"
#include "ext_ultra.h"
#include "ext_adc_dma.h"
#include "ext_uart_esp.h"
#include "string.h"
#include "usart.h"

static DHT11_Data_t dht11_data={0};
static Ultra_Data_t ultra_data={0};
static ADC_Data_t	adc_data  ={0};

static void Sensor_ProcessCmd(ProtocolFrame_t* frame){
	uint8_t response_data[64]={0};
	uint8_t data_len=0;
	
	if(frame->device_type!=DEVICE_TYPE_SENSOR)	return;
	printf("传感器命令: ID=%d,CMD=0x%02X\r\n",frame->device_id,frame->cmd);
	
	switch(frame->cmd){
		case SENSOR_CMD_GET_TEMP:{
			if(DHT11_Read(&dht11_data)){
				memcpy(response_data,&dht11_data.temperature,4);
				data_len=4;
				printf("温度:%.2f°C\r\n",dht11_data.temperature);
			}else {
				printf("DHT11读取温度失败\r\n");
			}
			break;
		}
		case SENSOR_CMD_GET_HUMI:{
			if(DHT11_Read(&dht11_data)){
				memcpy(response_data,&dht11_data.humidity,4);
				data_len=4;
				printf("湿度:%.2f%%\r\n",dht11_data.humidity);
			}else {
				printf("DHT11读取湿度失败\r\n");
			}
			break;
		}
		case SENSOR_CMD_GET_DIST:{
			if(Ultra_Read(&ultra_data)){
				memcpy(response_data,&ultra_data.distance_cm,4);
				data_len=4;
				printf("距离:%.2fcm\r\n",ultra_data.distance_cm);
			}else {
				printf("超声波读取距离失败\r\n");
			}
			break;
		}
		case SENSOR_CMD_GET_ALL:{
			float temp =-999,humi=-999,dist=-1;
			
			// 先检查 DHT11 是否在冷却中
			uint32_t now = xTaskGetTickCount();
			if ((now - dht11_data.last_read_time) < pdMS_TO_TICKS(2000)) {
				printf("DHT11 冷却中，跳过读取\r\n");
			} else if (DHT11_Read(&dht11_data)) {
				temp = dht11_data.temperature;
				humi = dht11_data.humidity;
			} else {
				printf("DHT11 读取超时/校验失败\r\n");
			}
			
			
			if(Ultra_Read(&ultra_data)){
				dist=ultra_data.distance_cm;
			}
			
			memcpy(&response_data[0],&temp,4);
			memcpy(&response_data[4],&humi,4);
			memcpy(&response_data[8],&dist,4);
			data_len=12;
			
			printf("温度:%.2f°C 湿度:%.2f%% 距离:%.2fcm\r\n", temp, humi, dist);
			break;
		}
		case SENSOR_CMD_GET_ADC:{
			//获取所有ADC通道
			if(ADC_DMA_Read(&adc_data)){
				for(uint8_t i=0;i<ADC_CHANNEL_COUNT;i++){
					memcpy(&response_data[i*4],&adc_data.voltages[i],4);
				}
				
				data_len = ADC_CHANNEL_COUNT*4;
				printf("ADC数据已读取，%d个通道\r\n",ADC_CHANNEL_COUNT);
			}else {
				printf("ADC数据未就绪\r\n");
			}
			break;
		}
		case SENSOR_CMD_GET_ADC_CH:{
			//获取指定ADC通道
			if(frame->data_len>=1){
				uint8_t ch=frame->data[0];
				if(ch<ADC_CHANNEL_COUNT+4){
					float voltage = ADC_GetVoltage(ch);
					memcpy(response_data,&voltage,4);
					data_len=4;
					printf("ADC通道%d:%.2fV\r\n",ch,voltage);
				}
			
			}
			break;
		}
		default:
			printf("未知命令: 0x%02X\r\n",frame->cmd);
			break;
	}
	if(data_len>0){
		ESP_UART_SendFrame(DEVICE_TYPE_SENSOR,DEVICE_ID_SENSOR,frame->cmd,data_len,response_data);
	}else {
		ESP_UART_SendFrame(DEVICE_TYPE_SENSOR,DEVICE_ID_SENSOR,frame->cmd,0,NULL);
	}
	
}

void vSensorTask(void *pvParameters){
	
	 ProtocolFrame_t frame = {0};
    
    // 初始化传感器
    ADC_DMA_Start();
    
    printf("传感器任务启动\r\n");
    printf("  DHT11: PA8\r\n");
    printf("  超声波: TRIG=PB12, ECHO=PB13\r\n");
    printf("  ADC: PA0~PA7, PB0, PB1 (DMA连续转换)\r\n");
    
    while(1) {
        // 检查ESP发送的命令
        if(ESP_UART_IsFrameReady()) {
            ESP_UART_ClearRxFlag();
            
            uint8_t* rx_buf = ESP_UART_GetRxBuffer();
            uint16_t rx_len = ESP_UART_GetRxLength();
            
            printf("收到ESP命令, len=%d\r\n", rx_len);
            for(int i = 0; i < rx_len; i++) {
                printf("%02X ", rx_buf[i]);
            }
            printf("\r\n");
            
            if(Protocol_UnpackFrame(rx_buf, rx_len, &frame)) {
                Sensor_ProcessCmd(&frame);
            } else {
                printf("帧解析失败, len=%d\r\n", rx_len);
            }
        }
        
        vTaskDelay(SENSOR_TASK_DELAY);
    }

}
