#include "ext_gpio_led.h"
#include "board_config.h"
#include "gpio.h"

void LED1_ON(void){
	HAL_GPIO_WritePin(LED1_PORT,LED1_PIN,GPIO_PIN_SET);

}

void LED1_OFF(void){
	HAL_GPIO_WritePin(LED1_PORT,LED1_PIN,GPIO_PIN_RESET);

}

void LED1_Turn(void){
	GPIO_PinState state=HAL_GPIO_ReadPin(LED1_PORT,LED1_PIN);
	HAL_GPIO_WritePin(LED1_PORT,LED1_PIN,!state);
}

