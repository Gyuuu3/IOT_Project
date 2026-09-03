#include "task_actuator.h"
#include "task_handle.h"
#include "debug_print.h"
#include "uart_actuator.h"
#include "task_config.h"
#include "protocol_frame.h"
#include "device_logic.h"
#include <string.h>

void vACTUA_Task(void *pvParameters){
    uint8_t recv_buffer[FRAME_MAX_SIZE * 2];
    ProtocolFrame_t frame = {0};
    uint8_t frame_buffer[FRAME_MAX_SIZE];
    uint16_t frame_index = 0;
    
    Debug_Print("执行器通信任务启动\r\n");
    
    while(1){
        // 处理发送队列
        MsgData_t tx_msg = {0};
        if(xQueueReceive(actua_msg_queue_handle, &tx_msg, 0) == pdPASS){
            UART_ACTUA_SendData(tx_msg.data, tx_msg.length);
            Debug_Print("发送到执行器STM32: ");
            for(int i = 0; i < tx_msg.length; i++){
                Debug_Print("%02X ", tx_msg.data[i]);
            }
            Debug_Print("\r\n");
        }
        
        // 从环形缓冲区读取数据
        int recv_len = UART_ACTUA_RecvData(recv_buffer, sizeof(recv_buffer), pdMS_TO_TICKS(1));
        
        if(recv_len > 0){
            Debug_Print("收到执行器数据(%d字节): ", recv_len);
            for(int i = 0; i < recv_len; i++){
                Debug_Print("%02X ", recv_buffer[i]);
            }
            Debug_Print("\r\n");
            
            // 逐字节处理
            for(int i = 0; i < recv_len; i++){
                frame_buffer[frame_index++] = recv_buffer[i];
                
                // 检查是否收到完整帧
                if(frame_index >= 2 && frame_buffer[frame_index-1] == FRAME_TAIL){
                    // 查找帧头位置
                    for(int j = 0; j < frame_index; j++){
                        if(frame_buffer[j] == FRAME_HEADER){
                            // 尝试解析从帧头开始的帧
                            uint16_t frame_len = frame_index - j;
                            if(Protocol_UnpackFrame(&frame_buffer[j], frame_len, &frame)){
                                Protocol_PrintFrame(&frame);
                                
                                // 如果是传感器数据
                                if(frame.device_type == DEVICE_TYPE_SENSOR){
                                    Device_ProcessSensorData(&frame);
                                }
                                break;
                            }
                        }
                    }
                    
                    // 检查缓冲区是否满了，如果满了就重置
                    if(frame_index >= FRAME_MAX_SIZE - 1){
                        // 保留最近的半个帧
                        uint16_t keep_len = frame_index - (frame_index / 2);
                        memmove(frame_buffer, &frame_buffer[frame_index - keep_len], keep_len);
                        frame_index = keep_len;
                    } else {
                        frame_index = 0;
                    }
                }
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}