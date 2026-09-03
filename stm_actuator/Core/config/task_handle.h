#ifndef __TASK_HANDLE_H__
#define __TASK_HANDLE_H__

#include "FreeRTOS.h"
#include "task.h"

/* 任务句柄 */
extern TaskHandle_t led_handle;
extern TaskHandle_t key_handle;
extern TaskHandle_t debug_handle;
extern TaskHandle_t esp_handle;
extern TaskHandle_t actua_handle;
/* 消息结构体 */
typedef struct{
	uint8_t *data;
	uint16_t length;
} MsgData_t;


#endif
