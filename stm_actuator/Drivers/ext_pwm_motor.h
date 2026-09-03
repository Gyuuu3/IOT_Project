#ifndef _EXT_PWM_MOTOR_H__
#define _EXT_PWM_MOTOR_H__
#include "stdint.h"

typedef enum{
	MOTOR_STOP=0,
	MOTOR_FORWARD,
	MOTOR_REVERSE
	
} MotorState_t;

void Motor1_SetState(MotorState_t state);
void Motor2_SetState(MotorState_t state);

void Motor1_SetSpeed(uint8_t speed);
void Motor2_SetSpeed(uint8_t speed);

#endif
