#ifndef __TASK_HANDLE_H__
#define __TASK_HANDLE_H__
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "board_config.h"

/* 任务句柄 */
extern TaskHandle_t wifi_task_handle;
extern TaskHandle_t mqtt_task_handle;
extern TaskHandle_t actua_task_handle;
extern TaskHandle_t sensor_task_handle;
extern TaskHandle_t debug_task_handle;
extern TaskHandle_t test_task_handle;

/* 消息队列句柄 */
extern QueueHandle_t mqtt_msg_queue_handle;
extern QueueHandle_t actua_msg_queue_handle; 
extern QueueHandle_t sensor_msg_queue_handle;

/* 通用消息结构体 */
typedef struct {
    uint8_t data[FRAME_MAX_SIZE];
    uint16_t length;
} MsgData_t;

/* 协议帧结构体 */
typedef struct {
    uint8_t head;           // 帧头 0xAA
    uint8_t device_type;    // 设备类型
    uint8_t device_id;      // 设备ID
    uint8_t cmd;            // 命令码
    uint8_t data_len;       // 数据长度
    uint8_t data[64];       // 数据区
    uint8_t tail;           // 帧尾 0xBB
} ProtocolFrame_t;

/* 传感器数据结构体 */
typedef struct {
    float temperature;      // 温度
    float humidity;         // 湿度
    float distance;         // 距离
    uint32_t timestamp;     // 时间戳
} SensorData_t;

#endif