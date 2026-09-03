#include "protocol_frame.h"
#include "board_config.h"
#include "debug_print.h"
#include <string.h>

/* 打包协议帧 */
void Protocol_PackFrame(uint8_t device_type, uint8_t device_id, 
                        uint8_t cmd, uint8_t *data, uint8_t data_len,
                        MsgData_t *out_msg){
    uint8_t idx = 0;
    
    if(data_len > 64) data_len = 64;
    
    out_msg->data[idx++] = FRAME_HEADER;
    out_msg->data[idx++] = device_type;
    out_msg->data[idx++] = device_id;
    out_msg->data[idx++] = cmd;
    out_msg->data[idx++] = data_len;
    
    if(data != NULL && data_len > 0){
        memcpy(&out_msg->data[idx], data, data_len);
        idx += data_len;
    }
    
    out_msg->data[idx++] = FRAME_TAIL;
    out_msg->length = idx;
}

/* 解包协议帧 */
bool Protocol_UnpackFrame(uint8_t *data, uint16_t length, 
                          ProtocolFrame_t *out_frame){
    if(length < 6) return false;  // 最小帧长度: HEAD(1)+TYPE(1)+ID(1)+CMD(1)+LEN(1)+TAIL(1)
    
    uint8_t idx = 0;
    
    // 查找帧头
    while(idx < length - 5){
        if(data[idx] == FRAME_HEADER){
            // 解析帧
            out_frame->head = data[idx++];
            out_frame->device_type = data[idx++];
            out_frame->device_id = data[idx++];
            out_frame->cmd = data[idx++];
            out_frame->data_len = data[idx++];
            
            // 检查数据长度
            if(out_frame->data_len > 64) return false;
            
            // 检查帧尾位置
            uint16_t data_end = idx + out_frame->data_len;
            if(data_end > length) return false;
            if(data[data_end] != FRAME_TAIL) return false;
            
            // 复制数据
            if(out_frame->data_len > 0){
                memcpy(out_frame->data, &data[idx], out_frame->data_len);
            }
            
            out_frame->tail = data[data_end];
            return true;
        }
        idx++;
    }
    return false;
}

/* 打印协议帧 */
void Protocol_PrintFrame(ProtocolFrame_t *frame){
    if(frame == NULL) return;
    
    Debug_Print("=== 协议帧解析 ===\n");
    Debug_Print("帧头: 0x%02X\n", frame->head);
    Debug_Print("设备类型: 0x%02X %s\n", frame->device_type, 
                frame->device_type == DEVICE_TYPE_ACTUATOR ? "执行器" : "传感器");
    Debug_Print("设备ID: %d\n", frame->device_id);
    Debug_Print("命令码: 0x%02X\n", frame->cmd);
    Debug_Print("数据长度: %d\n", frame->data_len);
    Debug_Print("数据: ");
    for(int i = 0; i < frame->data_len; i++){
        Debug_Print("%02X ", frame->data[i]);
    }
    Debug_Print("\n");
    Debug_Print("帧尾: 0x%02X\n", frame->tail);
    Debug_Print("================\n");
}
