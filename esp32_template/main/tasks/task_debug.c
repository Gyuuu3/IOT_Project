#include "task_debug.h"
#include "stdint.h"
#include "task_config.h"
#include "task_handle.h"
#include "debug_print.h"

void vDEBUG_Task(void *pvParameter){
    uint32_t tick_count = 0;
    UBaseType_t stack_wifi,stack_mqtt,stack_actua,stack_sensor,stack_test;

    Debug_Print("=====系统调试监控任务启动=====\r\n");

    while(1){
        /* 获取系统运行总时间 */
        tick_count =xTaskGetTickCount();

        /* 获取各任务剩余栈空间 */
        stack_wifi = uxTaskGetStackHighWaterMark(wifi_task_handle);
        stack_mqtt = uxTaskGetStackHighWaterMark(mqtt_task_handle);
        stack_actua = uxTaskGetStackHighWaterMark(actua_task_handle);
        stack_sensor = uxTaskGetStackHighWaterMark(sensor_task_handle);
        stack_test = uxTaskGetStackHighWaterMark(test_task_handle);
        
        /* 打印系统状态 */
        Debug_Print("[系统状态] Tick: %lu ms \r\n",tick_count);
        Debug_Print("   WIFI任务剩余栈空间: %d word | MQTT任务剩余栈空间 %d word\r\n",stack_wifi,stack_mqtt);
        Debug_Print("   执行器STM通信剩余栈空间:%d word | 传感器STM通信剩余栈空间:%d word\r\n",stack_actua,stack_sensor);
        Debug_Print("   test任务通信剩余栈空间:%d word \r\n",stack_test);
        Debug_Print("————————————————————————————————————————————————————————————————\r\n");

        vTaskDelay(DEBUG_TASK_DELAY);
    }

}
