#include "task_test.h"
#include "task_handle.h"
#include "debug_print.h"
#include "gpio_key.h"
#include "task_config.h"
#include "device_logic.h"
#include "protocol_frame.h"
#include "task_mqtt.h"

#define KEY_DEBOUNCE_TIME 50  // 防抖时间50ms

static bool key1_prev = false;
static bool key2_prev = false;
static bool key3_prev = false;
static uint32_t key1_last_time = 0;
static uint32_t key2_last_time = 0;
static uint32_t key3_last_time = 0;

void vTEST_Task(void *pvParameters){
    KEY_Init();
    Debug_Print("=== 测试任务启动 ===\r\n");
    Debug_Print("按键1: 发送传感器测试命令\r\n");
    Debug_Print("按键2: 发送执行器测试命令\r\n");
    Debug_Print("按键3: 发送MQTT测试消息\r\n");
    
    while(1){
        uint32_t current_time = xTaskGetTickCount();
        bool key1_state = KEY1_GetState();
        bool key2_state = KEY2_GetState();
        bool key3_state = KEY3_GetState();
        
        // 按键1: 发送传感器测试命令
        if(key1_state && !key1_prev && (current_time - key1_last_time) > KEY_DEBOUNCE_TIME){
            key1_last_time = current_time;
            Debug_Print("\r\n[测试] 按键1触发 - 发送传感器测试命令\r\n");
            
            // 发送获取所有传感器数据命令
            if(Device_SendToSensor(1, CMD_GET_ALL, NULL, 0)){
                Debug_Print("传感器测试命令已发送\r\n");
            }
        }
        
        // 按键2: 发送执行器测试命令
        if(key2_state && !key2_prev && (current_time - key2_last_time) > KEY_DEBOUNCE_TIME){
            key2_last_time = current_time;
            Debug_Print("\r\n[测试] 按键2触发 - 发送执行器测试命令\r\n");
            
            // LED闪烁测试: 1号LED翻转
            if(Device_SendToActuator(1, CMD_LED_TOGGLE, NULL, 0)){
                Debug_Print("执行器测试命令已发送\r\n");
            }
        }
        
        // 按键3: 发送MQTT测试消息
        if(key3_state && !key3_prev && (current_time - key3_last_time) > KEY_DEBOUNCE_TIME){
            key3_last_time = current_time;
            Debug_Print("\r\n[测试] 按键3触发 - 发送MQTT测试消息\r\n");
            
            char test_msg[256];
            snprintf(test_msg, sizeof(test_msg),
                    "{\"type\":\"test\",\"msg\":\"按键测试消息\",\"time\":%lu}",
                    current_time);
            Device_PublishData(test_msg);
            Debug_Print("MQTT测试消息已发送\r\n");
        }
        
        key1_prev = key1_state;
        key2_prev = key2_state;
        key3_prev = key3_state;
        
        vTaskDelay(TEST_TASK_DELAY);
    }
}