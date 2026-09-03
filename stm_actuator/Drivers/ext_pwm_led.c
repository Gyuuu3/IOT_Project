#include "ext_pwm_led.h"
#include "tim.h"
#include "board_config.h"

void LED2_SetBright(uint8_t bright){
	if(bright> 100) bright=100;
	__HAL_TIM_SET_COMPARE(&LED2_TIM_HANDLE,LED2_TIM_CHANNEL,bright*10);
}




