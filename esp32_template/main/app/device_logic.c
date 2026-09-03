#include "device_logic.h"
#include "protocol_frame.h"
#include "debug_print.h"
#include <string.h>
#include "board_config.h"
#include "task_mqtt.h"

/* 解析MQTT下发的命令 - 支持新协议格式 */
void Device_ParseMQTTCmd(uint8_t *data, uint16_t length){
    Debug_Print("云端下发命令: %.*s\r\n", length, data);

    // 解析JSON格式命令
    const char* start = strstr((char*)data, "\"content\":\"");
    if(start == NULL){
        Debug_Print("未找到content字段\r\n");
        return;
    }
    
    start += 11;
    const char* end = strchr(start, '"');
    if(end == NULL){
        Debug_Print("命令格式错误\r\n");
        return;
    }
    
    int cmd_len = end - start;
    char cmd[64] = {0};
    if(cmd_len >= sizeof(cmd)) cmd_len = sizeof(cmd) - 1;
    memcpy(cmd, start, cmd_len);
    cmd[cmd_len] = '\0';

    Debug_Print("解析后的命令: %s\r\n", cmd);

    char type_str[16] = {0};
    char id_str[8] = {0};
    char cmd_str[16] = {0};
    char param_str[32] = {0};
    
    int parsed = sscanf(cmd, "%s %s %s %[^\n]", type_str, id_str, cmd_str, param_str);
    
    if(parsed < 3){
        Debug_Print("命令格式错误，需要: 设备类型 设备ID 命令 [参数]\r\n");
        return;
    }
    
    uint8_t device_id = atoi(id_str);
    uint8_t device_type = 0;
    
    if(strcmp(type_str, "led") == 0 || strcmp(type_str, "motor") == 0 || strcmp(type_str, "servo") == 0){
        device_type = DEVICE_TYPE_ACTUATOR;
    } else if(strcmp(type_str, "temp") == 0 || strcmp(type_str, "humid") == 0 || 
              strcmp(type_str, "distance") == 0 || strcmp(type_str, "sensor") == 0){
        device_type = DEVICE_TYPE_SENSOR;
    } else {
        Debug_Print("未知设备类型: %s\r\n", type_str);
        return;
    }
    
    uint8_t data_buf[64] = {0};
    uint8_t data_len = 0;
    
    if(parsed >= 4){
        char *token = strtok(param_str, " ");
        while(token != NULL && data_len < 64){
            data_buf[data_len++] = atoi(token);
            token = strtok(NULL, " ");
        }
    }
    
    uint8_t cmd_code = 0;
    if(strcmp(type_str, "led") == 0){
        if(strcmp(cmd_str, "on") == 0)              cmd_code = CMD_LED_ON;
        else if(strcmp(cmd_str, "off") == 0)        cmd_code = CMD_LED_OFF;
        else if(strcmp(cmd_str, "toggle") == 0)     cmd_code = CMD_LED_TOGGLE;
    } else if(strcmp(type_str, "motor") == 0){
        if(strcmp(cmd_str, "run") == 0)             cmd_code = CMD_MOTOR_RUN;
        else if(strcmp(cmd_str, "stop") == 0)       cmd_code = CMD_MOTOR_STOP;
        else if(strcmp(cmd_str, "reversal") == 0)   cmd_code = CMD_MOTOR_REV;
    } else if(strcmp(type_str,"servo") == 0){
        if(strcmp(cmd_str,"forward") == 0)          cmd_code = CMD_SERVO_FWD;
        else if(strcmp(cmd_str, "reversal") == 0)   cmd_code = CMD_SERVO_REV;
        else if(strcmp(cmd_str, "reset") == 0)      cmd_code = CMD_SERVO_RST;
    } else if(strcmp(type_str, "temp") == 0 || strcmp(type_str, "sensor") == 0){
        if(strcmp(cmd_str, "get") == 0)             cmd_code = CMD_GET_ALL;
        else if(strcmp(cmd_str, "temp") == 0)       cmd_code = CMD_GET_TEMP;
        else if(strcmp(cmd_str, "humid") == 0)      cmd_code = CMD_GET_HUMID;
        else if(strcmp(cmd_str, "distance") == 0)   cmd_code = CMD_GET_DIST;
    }
    
    if(cmd_code == 0){
        Debug_Print("未知命令: %s\r\n", cmd_str);
        return;
    }
    
    if(device_type == DEVICE_TYPE_ACTUATOR){
        Device_SendToActuator(device_id, cmd_code, data_buf, data_len);
    } else {
        Device_SendToSensor(device_id, cmd_code, data_buf, data_len);
    }
}

void Device_PublishData(const char *data){
    esp_mqtt_client_handle_t mqtt_client = mqtt_get_client();
    if(mqtt_client == NULL){
        Debug_Print("MQTT客户端未初始化,无法发布消息\r\n");
        return;
    }

    int msg_id = esp_mqtt_client_publish(mqtt_client, MQTT_PUBLISH_TOPIC, data, 0, 0, 0);
    if(msg_id != -1){
        Debug_Print("消息发布成功,topic=%s,msg_id=%d\r\n", MQTT_PUBLISH_TOPIC, msg_id);
    } else {
        Debug_Print("消息发布失败\r\n");
    }
}


void Device_ProcessSensorData(ProtocolFrame_t *frame){
    if(frame->device_type != DEVICE_TYPE_SENSOR) return;
    
    SensorData_t sensor_data = {0};
    sensor_data.timestamp = xTaskGetTickCount();
    
    switch(frame->cmd){
        case CMD_GET_TEMP:
            if(frame->data_len >= 4){
                memcpy(&sensor_data.temperature, frame->data, 4);
                Debug_Print("温度: %.2f°C\r\n", sensor_data.temperature);
            }
            break;
        case CMD_GET_HUMID:
            if(frame->data_len >= 4){
                memcpy(&sensor_data.humidity, frame->data, 4);
                Debug_Print("湿度: %.2f%%\r\n", sensor_data.humidity);
            }
            break;
        case CMD_GET_DIST:
            if(frame->data_len >= 4){
                memcpy(&sensor_data.distance, frame->data, 4);
                Debug_Print("距离: %.2fcm\r\n", sensor_data.distance);
            }
            break;
        case CMD_GET_ALL:
            if(frame->data_len >= 12){
                memcpy(&sensor_data.temperature, &frame->data[0], 4);
                memcpy(&sensor_data.humidity, &frame->data[4], 4);
                memcpy(&sensor_data.distance, &frame->data[8], 4);
                Debug_Print("综合数据 - 温度:%.2f°C 湿度:%.2f%% 距离:%.2fcm\r\n", 
                           sensor_data.temperature, sensor_data.humidity, sensor_data.distance);
                
                char json_buf[256];
                snprintf(json_buf, sizeof(json_buf), 
                        "{\"type\":\"sensor\",\"temp\":%.2f,\"humid\":%.2f,\"dist\":%.2f}",
                        sensor_data.temperature, sensor_data.humidity, sensor_data.distance);
                Device_PublishData(json_buf);
            }
            break;
        // 【新增】处理传感器主动上报的按键事件
        case 0x20: {  // 自定义：传感器按键事件命令码
            if(frame->data_len >= 1) {
                uint8_t key_id = frame->data[0];
                Debug_Print("【传感器上报】按键%d按下\r\n", key_id);
                // 可上报到云端
                char json_buf[128];
                snprintf(json_buf, sizeof(json_buf), 
                        "{\"type\":\"key_event\",\"sensor_id\":%d,\"key\":%d}",
                        frame->device_id, key_id);
                Device_PublishData(json_buf);
            }
            break;
        }
        default:
            Debug_Print("未知传感器命令: 0x%02X\r\n", frame->cmd);
            break;
    }
}

/* 发送命令到执行器STM32 */
bool Device_SendToActuator(uint8_t device_id, uint8_t cmd, uint8_t *data, uint8_t data_len){
    MsgData_t msg = {0};
    Protocol_PackFrame(DEVICE_TYPE_ACTUATOR, device_id, cmd, data, data_len, &msg);
    
    if(xQueueSend(actua_msg_queue_handle, &msg, pdMS_TO_TICKS(100)) == pdPASS){
        Debug_Print("命令已发送到执行器队列\r\n");
        return true;
    } else {
        Debug_Print("【错误】发送到执行器队列失败！队列可能已满或句柄无效\r\n");
        return false;
    }
}

/* 发送命令到传感器STM32 */
bool Device_SendToSensor(uint8_t device_id, uint8_t cmd, uint8_t *data, uint8_t data_len){
    MsgData_t msg = {0};
    Protocol_PackFrame(DEVICE_TYPE_SENSOR, device_id, cmd, data, data_len, &msg);
    
    if(sensor_msg_queue_handle == NULL){
        Debug_Print("【错误】sensor_msg_queue_handle 为 NULL！\r\n");
        return false;
    }
    
    if(xQueueSend(sensor_msg_queue_handle, &msg, pdMS_TO_TICKS(100)) == pdPASS){
        Debug_Print("命令已发送到传感器队列\r\n");
        return true;
    } else {
        Debug_Print("【错误】发送到传感器队列失败！队列可能已满\r\n");
        return false;
    }
}
