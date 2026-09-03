#include "ext_pwm_servo.h"
#include "tim.h"
#include "board_config.h"

void Servo1_SetAngle(uint16_t angle){
	if(angle>180)	angle=180;
	uint16_t pulse=500+(angle*2000/180);
	__HAL_TIM_SET_COMPARE(&SERVO1_TIM_HANDLE,SERVO1_TIM_CHANNEL,pulse);

}

void Servo2_SetAngle(uint16_t angle){
	if(angle>180)	angle=180;
	uint16_t pulse=500+(angle*2000/180);
	__HAL_TIM_SET_COMPARE(&SERVO2_TIM_HANDLE,SERVO2_TIM_CHANNEL,pulse);

}
