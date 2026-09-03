#include "task_key.h"
#include "board_config.h"
#include "task_config.h"
#include "usart.h"
#include "ext_gpio_key.h"
#include "ext_uart_esp.h"

void vKeyTask(void* pvParameters){
	uint8_t key=0;
	printf("按键检测任务启动\r\n");
	while(1){
		key=Key_Scan();
		if(key==1){
			printf("按键1按下\r\n");
			ESP_UART_Send("key1pressed",sizeof("key1pressed"));
		}
		vTaskDelay(KEY_TASK_DELAY);
	}
	
}



