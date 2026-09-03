#include "system_init.h"
#include "nvs_flash.h"
#include "gpio_led.h"
#include "gpio_key.h"
#include "uart_actuator.h"
#include "uart_sensor.h"
#include "task_config.h"
#include "task_handle.h"
#include "task_wifi.h"
#include "task_mqtt.h"
#include "task_actuator.h"
#include "task_sensor.h"
#include "task_debug.h"
#include "task_test.h"

/* 全局句柄定义 */
TaskHandle_t wifi_task_handle = NULL;
TaskHandle_t mqtt_task_handle = NULL;
TaskHandle_t actua_task_handle = NULL;
TaskHandle_t sensor_task_handle = NULL;
TaskHandle_t debug_task_handle = NULL;
TaskHandle_t test_task_handle = NULL;

QueueHandle_t mqtt_msg_queue_handle = NULL;
QueueHandle_t actua_msg_queue_handle = NULL;
QueueHandle_t sensor_msg_queue_handle = NULL;

void System_Hardware_Init(void){

    // ========== 2. 初始化NVS ==========
    esp_err_t ret = nvs_flash_init();
    if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // ========== 3. 初始化硬件 ==========
    LED_Init();
    KEY_Init();
    UART_ACTUA_Init();
    UART_SENSOR_Init();  
    
    // ========== 4. 初始化消息队列 ==========
    mqtt_msg_queue_handle = xQueueCreate(MQTT_MSG_QUEUE_LEN, sizeof(MsgData_t));
    actua_msg_queue_handle = xQueueCreate(ACTUA_MSG_QUEUE_LEN, sizeof(MsgData_t));
    sensor_msg_queue_handle = xQueueCreate(SENSOR_MSG_QUEUE_LEN, sizeof(MsgData_t));
    
    printf("=== 系统初始化完成 ===\n");
}

void RTOS_Create_ALL_Tasks(void){
    // 创建所有任务
    xTaskCreate(vWIFI_Task, "WiFi_Task", TASK_STACK_SIZE_WIFI, NULL, TASK_PRIO_WIFI, &wifi_task_handle);
    xTaskCreate(vMQTT_Task, "MQTT_Task", TASK_STACK_SIZE_MQTT, NULL, TASK_PRIO_MQTT, &mqtt_task_handle);
    xTaskCreate(vACTUA_Task, "ACTUA_Task", TASK_STACK_SIZE_ACTUA, NULL, TASK_PRIO_ACTUA, &actua_task_handle);
    xTaskCreate(vSENSOR_Task, "SENSOR_Task", TASK_STACK_SIZE_SENSOR, NULL, TASK_PRIO_SENSOR, &sensor_task_handle);
    xTaskCreate(vDEBUG_Task, "Debug_Task", TASK_STACK_SIZE_DEBUG, NULL, TASK_PRIO_DEBUG, &debug_task_handle);
    xTaskCreate(vTEST_Task, "Test_Task", TASK_STACK_SIZE_TEST, NULL, TASK_PRIO_TEST, &test_task_handle);
}