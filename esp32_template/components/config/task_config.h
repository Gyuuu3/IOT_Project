#ifndef __TASK_CONFIG_H__
#define __TASK_CONFIG_H__
#include "freertos/FreeRTOS.h"

/* 优先级 */
#define TASK_PRIO_WIFI                  5
#define TASK_PRIO_MQTT                  4
#define TASK_PRIO_ACTUA                 3
#define TASK_PRIO_SENSOR                3
#define TASK_PRIO_DEBUG                 1
#define TASK_PRIO_TEST                  2

/* 栈大小 */
#define TASK_STACK_SIZE_WIFI            4096
#define TASK_STACK_SIZE_MQTT            4096
#define TASK_STACK_SIZE_ACTUA           4096
#define TASK_STACK_SIZE_SENSOR          4096
#define TASK_STACK_SIZE_DEBUG           4096
#define TASK_STACK_SIZE_TEST            4096

/* 任务延时 */
#define WIFI_TASK_DELAY                 pdMS_TO_TICKS(3000)
#define MQTT_TASK_DELAY                 pdMS_TO_TICKS(1000)
#define ACTUA_TASK_DELAY                pdMS_TO_TICKS(100)
#define SENSOR_TASK_DELAY               pdMS_TO_TICKS(100)
#define DEBUG_TASK_DELAY                pdMS_TO_TICKS(10000)
#define TEST_TASK_DELAY                 pdMS_TO_TICKS(20)

/* 队列长度 */
#define MQTT_MSG_QUEUE_LEN              8
#define ACTUA_MSG_QUEUE_LEN             8
#define SENSOR_MSG_QUEUE_LEN            8

#endif