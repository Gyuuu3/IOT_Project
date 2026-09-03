#include "task_led.h"
#include "board_config.h"
#include "task_config.h"
#include "usart.h"
#include "gpio.h"

void vLedTask(void* pvParameters){
	uint8_t led_state = 0;
	printf("LED任务启动\r\n");
	while(1){
		/*led_state = !led_state;
		HAL_GPIO_WritePin(LED_PORT,LED_PIN,(GPIO_PinState)led_state);*/
		vTaskDelay(LED_TASK_DELAY);
	}
	
}
