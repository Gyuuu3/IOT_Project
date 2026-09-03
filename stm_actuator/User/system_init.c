#include "system_init.h"

/* cubemx底层驱动文件 */
#include "main.h"
#include "dma.h"
#include "gpio.h"
#include "usart.h"
#include "tim.h"

/* 手写任务 */
#include "task_led.h"
#include "task_key.h"
#include "task_debug.h"
#include "task_esp.h"
#include "task_actuator.h"

/* 配置 */
#include "task_config.h"

#include "FreeRTOS.h"
#include "Task.h"

TaskHandle_t led_handle;
TaskHandle_t key_handle;
TaskHandle_t debug_handle;
TaskHandle_t esp_handle;
TaskHandle_t actua_handle;

void System_Hardware_Init(void){
	HAL_Init();
	SystemClock_Config();
	
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_TIM3_Init();
	MX_TIM4_Init();
	
}

void RTOS_Create_ALL_Tasks(void){
	xTaskCreate(vLedTask,"led_Task",LED_STACK,NULL,LED_TASK_PRIO,&led_handle);
	xTaskCreate(vKeyTask,"key_Task",KEY_STACK,NULL,KEY_TASK_PRIO,&key_handle);
	xTaskCreate(vDebugTask,"debug_Task",DEBUG_STACK,NULL,DEBUG_TASK_PRIO,&debug_handle);
	xTaskCreate(vESPTask,"esp_Task",ESP_STACK,NULL,ESP_TASK_PRIO,&esp_handle);
	xTaskCreate(vACTUATask,"actua_Task",ACTUA_STACK,NULL,ACTUA_TASK_PRIO,&actua_handle);
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    // 串口打印溢出任务名，若输出actua_Task则实锤栈溢出
    printf("Stack Overflow Task:%s\r\n",pcTaskName);
    while(1);
}
void vApplicationMallocFailedHook(void)
{
    printf("Malloc Failed! Heap insufficient\r\n");
    while(1);
}
