#include "system_init.h"

/* cubemx底层驱动文件 */
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"
/* 手写任务 */
#include "task_led.h"
#include "task_key.h"
#include "task_debug.h"
#include "task_sensor.h"

/* 配置 */
#include "task_config.h"
#include "FreeRTOS.h"
#include "Task.h"

TaskHandle_t led_handle;
TaskHandle_t key_handle;
TaskHandle_t debug_handle;
TaskHandle_t sensor_handle;

void System_Hardware_Init(void){
	HAL_Init();
	SystemClock_Config();
	
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_ADC1_Init();
	MX_TIM4_Init();
	
	HAL_TIM_Base_Start(&htim4);
	
}

void RTOS_Create_ALL_Tasks(void){
	xTaskCreate(vLedTask,"led_Task",LED_STACK,NULL,LED_TASK_PRIO,&led_handle);
	xTaskCreate(vKeyTask,"key_Task",KEY_STACK,NULL,KEY_TASK_PRIO,&key_handle);
	xTaskCreate(vDebugTask,"debug_Task",DEBUG_STACK,NULL,DEBUG_TASK_PRIO,&debug_handle);
	xTaskCreate(vSensorTask,"sensor_Task",SENSOR_STACK,NULL,SENSOR_TASK_PRIO,&sensor_handle);
	
}

/*
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    // 栈溢出卡死，直接打印故障任务
    printf("Stack Overflow Task: %s\r\n", pcTaskName);
    while(1);
}
*/
