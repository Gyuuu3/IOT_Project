#include "ext_gpio_key.h"
#include "board_config.h"
#include "gpio.h"
#include "FreeRTOS.h"

uint8_t Key_Scan(void){
	uint8_t keynum=0;
	if(HAL_GPIO_ReadPin(KEY1_PORT,KEY1_PIN)==KEY_DOWN){
		HAL_Delay(20);
		if(HAL_GPIO_ReadPin(KEY1_PORT,KEY1_PIN)==KEY_DOWN){
			keynum=1;
		}
	}
	return keynum;

}


