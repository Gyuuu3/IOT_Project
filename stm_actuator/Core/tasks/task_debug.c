#include "task_debug.h"
#include "board_config.h"
#include "task_config.h"
#include "usart.h"

void vDebugTask(void* pvParameters){
	//uint32_t tick_cnt=0;
	printf("调试打印任务启动\r\n");
	while(1){
		
		vTaskDelay(DEBUG_TASK_DELAY);
	}

}


