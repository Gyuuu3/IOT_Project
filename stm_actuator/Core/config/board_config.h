#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#include "main.h"

/* ========== GPIO引脚定义 ========== */
// LED
#define LED1_PORT       		GPIOA
#define LED1_PIN        		GPIO_PIN_1
#define LED2_PORT       		GPIOB
#define LED2_PIN        		GPIO_PIN_0
#define LED2_TIM_HANDLE			htim3
#define LED2_TIM_CHANNEL		TIM_CHANNEL_3

// KEY
#define KEY1_PORT       		GPIOA
#define KEY1_PIN        		GPIO_PIN_0
#define KEY_DOWN        0
#define KEY_UP          1

// 电机 (PWM)
#define MOTOR1_PORT     		GPIOA
#define MOTOR1_PIN      		GPIO_PIN_6      // TIM3_CH1
#define MOTOR2_PORT     		GPIOA
#define MOTOR2_PIN      		GPIO_PIN_7      // TIM3_CH2

//控制引脚
#define MOTOR1_IN1_PORT     		GPIOB
#define MOTOR1_IN1_PIN      		GPIO_PIN_12
#define MOTOR1_IN2_PORT     		GPIOB
#define MOTOR1_IN2_PIN      		GPIO_PIN_13
#define MOTOR2_IN1_PORT     		GPIOB
#define MOTOR2_IN1_PIN      		GPIO_PIN_14
#define MOTOR2_IN2_PORT     		GPIOB
#define MOTOR2_IN2_PIN      		GPIO_PIN_15

#define MOTOR1_TIM_HANDLE		htim3
#define MOTOR1_TIM_CHANNEL		TIM_CHANNEL_1
#define MOTOR2_TIM_HANDLE		htim3
#define MOTOR2_TIM_CHANNEL		TIM_CHANNEL_2

// 舵机 (PWM)
#define SERVO1_PORT     		GPIOB
#define SERVO1_PIN      		GPIO_PIN_8      // TIM4_CH3
#define SERVO2_PORT     		GPIOB
#define SERVO2_PIN      		GPIO_PIN_9      // TIM4_CH4

#define SERVO1_TIM_HANDLE		htim4
#define SERVO1_TIM_CHANNEL		TIM_CHANNEL_3
#define SERVO2_TIM_HANDLE		htim4
#define SERVO2_TIM_CHANNEL		TIM_CHANNEL_4


/* ========== 串口配置 ========== */
#define DEBUG_UART_HANDLE   huart1

/* ESP32通信串口 UART2 */
#define ESP_UART_HANDLE     huart2
#define ESP_UART_BAUD       115200

/* ========== 通信帧协议  ========== */
#define FRAME_HEAD        	0xAA
#define FRAME_TAIL          0xBB
#define FRAME_MAX_SIZE      128

/* ========== 设备类型  ========== */
#define DEVICE_TYPE_ACTUATOR   0x01
#define DEVICE_TYPE_SENSOR     0x02

/* ========== 执行器命令码  ========== */
#define ACTUATOR_CMD_LED_ON      0x01
#define ACTUATOR_CMD_LED_OFF     0x02
#define ACTUATOR_CMD_LED_TOGGLE  0x03
#define ACTUATOR_CMD_MOTOR_RUN   0x04
#define ACTUATOR_CMD_MOTOR_STOP  0x05
#define ACTUATOR_CMD_MOTOR_REV   0x06

/* ========== 设备ID ========== */
#define DEVICE_ID_ACTUATOR  1

#endif
