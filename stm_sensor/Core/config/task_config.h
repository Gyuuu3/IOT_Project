#ifndef __TASK_CONFIG_H__
#define __TASK_CONFIG_H__

#include "FreeRTOS.h"
#include "task.h"

/* 任务优先级 */
#define LED_TASK_PRIO           1
#define KEY_TASK_PRIO           2
#define DEBUG_TASK_PRIO         1
#define SENSOR_TASK_PRIO        4

/* 任务栈 */
#define LED_STACK               256
#define KEY_STACK               256
#define DEBUG_STACK             512
#define SENSOR_STACK            1024

/* 任务延时 */
#define LED_TASK_DELAY          pdMS_TO_TICKS(500)
#define KEY_TASK_DELAY          pdMS_TO_TICKS(20)
#define DEBUG_TASK_DELAY        pdMS_TO_TICKS(1000)
#define SENSOR_TASK_DELAY       pdMS_TO_TICKS(10)

/* 传感器采集间隔 */
#define DHT11_READ_INTERVAL     pdMS_TO_TICKS(2000)   // DHT11 2秒一次
#define ULTRA_READ_INTERVAL     pdMS_TO_TICKS(200)    // 超声波 200ms一次
#define ADC_READ_INTERVAL       pdMS_TO_TICKS(500)    // ADC 500ms一次

#endif

