#include "task_sensor.h"
#include "task_handle.h"
#include "debug_print.h"
#include "uart_sensor.h"
#include "task_config.h"
#include "protocol_frame.h"
#include "device_logic.h"
#include <string.h>

void vSENSOR_Task(void *pvParameters){
    uint8_t recv_buffer[FRAME_MAX_SIZE * 2];
    ProtocolFrame_t frame = {0};
    uint8_t frame_buffer[FRAME_MAX_SIZE];
    uint16_t frame_index = 0;
    
    Debug_Print("传感器通信任务启动\r\n");
    
    while(1){
        // 处理发送队列（ESP32 -> 传感器STM32 命令）
        MsgData_t tx_msg = {0};
        if(xQueueReceive(sensor_msg_queue_handle, &tx_msg, 0) == pdPASS){
            UART_SENSOR_SendData(tx_msg.data, tx_msg.length);
            Debug_Print("发送到传感器STM32: ");
            for(int i = 0; i < tx_msg.length; i++){
                Debug_Print("%02X ", tx_msg.data[i]);
            }
            Debug_Print("\r\n");
        }
        
        // 接收传感器数据（传感器STM32 -> ESP32）
        int recv_len = UART_SENSOR_RecvData(recv_buffer, sizeof(recv_buffer), pdMS_TO_TICKS(10));
        
        if(recv_len > 0){
            Debug_Print("收到传感器数据(%d字节): ", recv_len);
            for(int i = 0; i < recv_len; i++){
                Debug_Print("%02X ", recv_buffer[i]);
            }
            Debug_Print("\r\n");
            
            // 逐字节解析，支持粘包处理
            for(int i = 0; i < recv_len; i++){
                frame_buffer[frame_index++] = recv_buffer[i];
                
                if(frame_index >= 2 && frame_buffer[frame_index-1] == FRAME_TAIL){
                    for(int j = 0; j < frame_index; j++){
                        if(frame_buffer[j] == FRAME_HEADER){
                            uint16_t frame_len = frame_index - j;
                            if(Protocol_UnpackFrame(&frame_buffer[j], frame_len, &frame)){
                                Protocol_PrintFrame(&frame);
                                // 处理传感器数据（包括主动上报）
                                Device_ProcessSensorData(&frame);
                                break;
                            }
                        }
                    }
                    
                    if(frame_index >= FRAME_MAX_SIZE - 1){
                        uint16_t keep_len = frame_index - (frame_index / 2);
                        memmove(frame_buffer, &frame_buffer[frame_index - keep_len], keep_len);
                        frame_index = keep_len;
                    } else {
                        frame_index = 0;
                    }
                }
            }
        }
        
        vTaskDelay(SENSOR_TASK_DELAY);
    }
}