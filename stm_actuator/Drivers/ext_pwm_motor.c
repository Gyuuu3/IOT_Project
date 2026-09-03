#include "ext_pwm_motor.h"
#include "tim.h"
#include "board_config.h"

static void Motor_SetPins(GPIO_TypeDef* in1_port, uint16_t in1_pin,
                          GPIO_TypeDef* in2_port, uint16_t in2_pin,
                          MotorState_t state)
{
    switch(state) {
        case MOTOR_STOP:
            HAL_GPIO_WritePin(in1_port, in1_pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(in2_port, in2_pin, GPIO_PIN_RESET);
            break;
        case MOTOR_FORWARD:
            HAL_GPIO_WritePin(in1_port, in1_pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(in2_port, in2_pin, GPIO_PIN_RESET);
            break;
        case MOTOR_REVERSE:
            HAL_GPIO_WritePin(in1_port, in1_pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(in2_port, in2_pin, GPIO_PIN_SET);
            break;
	}
       
}

void Motor1_SetState(MotorState_t state){
	Motor_SetPins(MOTOR1_IN1_GPIO_Port,MOTOR1_IN1_PIN,
					MOTOR1_IN2_GPIO_Port,MOTOR1_IN2_PIN,state);
}

void Motor2_SetState(MotorState_t state){
	Motor_SetPins(MOTOR2_IN1_GPIO_Port,MOTOR2_IN1_PIN,
					MOTOR2_IN2_GPIO_Port,MOTOR2_IN2_PIN,state);
}

void Motor1_SetSpeed(uint8_t speed){
	if(speed>100)	speed=100;
	__HAL_TIM_SET_COMPARE(&MOTOR1_TIM_HANDLE,MOTOR1_TIM_CHANNEL,speed*10);

}
	
void Motor2_SetSpeed(uint8_t speed){
	if(speed>100)	speed=100;
	__HAL_TIM_SET_COMPARE(&MOTOR2_TIM_HANDLE,MOTOR2_TIM_CHANNEL,speed*10);

}	

