#ifndef __TASK_CONFIG_H__
#define __TASK_CONFIG_H__
#include "FreeRTOS.h"
#include "task.h"

/* 任务优先级,数值大优先级高 */
#define LED_TASK_PRIO		2
#define KEY_TASK_PRIO		3
#define DEBUG_TASK_PRIO		1
#define ESP_TASK_PRIO		3
#define ACTUA_TASK_PRIO		3


/* 任务栈 */
#define	LED_STACK			512
#define KEY_STACK			512
#define DEBUG_STACK			1024
#define ESP_STACK			512 
#define ACTUA_STACK			1024

/* 任务延时 */
#define LED_TASK_DELAY		pdMS_TO_TICKS(500)
#define KEY_TASK_DELAY		pdMS_TO_TICKS(20)
#define DEBUG_TASK_DELAY	pdMS_TO_TICKS(1000)
#define ESP_TASK_DELAY		pdMS_TO_TICKS(10)
#define ACTUA_TASK_DELAY	pdMS_TO_TICKS(10)


#endif
