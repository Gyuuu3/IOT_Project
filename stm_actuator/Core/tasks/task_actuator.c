#include "task_actuator.h"
#include "task_config.h"
#include "board_config.h"
#include "protocol_frame.h"
#include "ext_pwm_led.h"
#include "ext_pwm_motor.h"
#include "ext_pwm_servo.h"
#include "ext_gpio_led.h"
#include "ext_uart_esp.h"
#include "tim.h"
#include "usart.h"
#include "string.h"

/* 执行器命令处理函数 */
static void Actuator_ProcessCmd(ProtocolFrame_t *frame)
{
    if(frame->device_type != DEVICE_TYPE_ACTUATOR) return;
    
    printf("执行器命令: ID=%d, CMD=0x%02X, Len=%d\r\n", 
           frame->device_id, frame->cmd, frame->data_len);
	
    uint8_t device_id = frame->device_id;
	uint8_t value=0,direction=0;
	
	if(frame->data_len>=1)	value =frame->data[0];
	
	if(frame->data_len>=2)	direction = frame->data[1];
	printf("id=%d value=%d direction=%d \n",device_id,value,direction);
    switch(frame->cmd) {
        case ACTUATOR_CMD_LED_ON: {
            // data[0]=亮度(0-100)
            if(frame->data_len >= 1) {

                if(device_id == 2) {
                    LED2_SetBright(value);
                    printf("LED2 亮度: %d%%\r\n", value);
                }
            } else {
				if(device_id==1)	LED1_ON();
				if(device_id==2)	LED2_SetBright(100);
                printf("LED%d 开启\n",device_id);
            }
            
            // 发送ACK
            ESP_UART_SendFrame(DEVICE_TYPE_ACTUATOR, DEVICE_ID_ACTUATOR,
                              frame->cmd, 0, NULL);
            break;
        }
        
        case ACTUATOR_CMD_LED_OFF: {
			if(device_id==1)	LED1_OFF();
			if(device_id==2)	LED2_SetBright(0);
			
            printf("LED 关闭\r\n");
            ESP_UART_SendFrame(DEVICE_TYPE_ACTUATOR, DEVICE_ID_ACTUATOR,
                              frame->cmd, 0, NULL);
            break;
        }
        
        case ACTUATOR_CMD_LED_TOGGLE: {
			if(device_id==1)	LED1_Turn();
			if(device_id==2){	
				if(value>0)	value=0;
				else if(value==0)	value=100;
				LED2_SetBright(value);
			}
            
            printf("LED%d 翻转\n",device_id);
            ESP_UART_SendFrame(DEVICE_TYPE_ACTUATOR, DEVICE_ID_ACTUATOR,
                              frame->cmd, 0, NULL);
            break;
        }

        case ACTUATOR_CMD_MOTOR_RUN: {
            if(frame->data_len >= 1) {
                if(device_id == 1) {
                    Motor1_SetState(direction ? MOTOR_REVERSE : MOTOR_FORWARD);
                    Motor1_SetSpeed(value);
                    printf("电机1 %s 速度: %d%%\r\n", 
                           direction ? "反转" : "正转", value);
                } else if(device_id == 2) {
                    Motor2_SetState(direction ? MOTOR_REVERSE : MOTOR_FORWARD);
                    Motor2_SetSpeed(value);
                    printf("电机2 %s 速度: %d%%\r\n", 
                           direction ? "反转" : "正转", value);
                }
            } else {
                Motor1_SetState(MOTOR_FORWARD);
                Motor1_SetSpeed(50);
                printf("电机1 正转 速度: 50%%(默认)\r\n");
            }
            ESP_UART_SendFrame(DEVICE_TYPE_ACTUATOR, DEVICE_ID_ACTUATOR,
                              frame->cmd, 0, NULL);
            break;
        }
        
        case ACTUATOR_CMD_MOTOR_STOP: {
                if(device_id == 1) {
                    Motor1_SetState(MOTOR_STOP);
                    Motor1_SetSpeed(0);
                } else if(device_id == 2) {
                    Motor2_SetState(MOTOR_STOP);
                    Motor2_SetSpeed(0);
                }
            printf("电机%d停止\n",device_id);
            ESP_UART_SendFrame(DEVICE_TYPE_ACTUATOR, DEVICE_ID_ACTUATOR,
                              frame->cmd, 0, NULL);
            break;
        }
        
        case ACTUATOR_CMD_MOTOR_REV: {
            if(frame->data_len >= 1) {             
                if(device_id == 1) {
                    Motor1_SetState(MOTOR_REVERSE);
                    Motor1_SetSpeed(value);
                    printf("电机1 反转 速度: %d%%\r\n", value);
                } else if(device_id == 2) {
                    Motor2_SetState(MOTOR_REVERSE);
                    Motor2_SetSpeed(value);
                    printf("电机2 反转 速度: %d%%\r\n", value);
                }
            } else {
                Motor1_SetState(MOTOR_REVERSE);
                Motor1_SetSpeed(50);
                printf("电机1 反转 速度: 50%%(默认)\r\n");
            }
            ESP_UART_SendFrame(DEVICE_TYPE_ACTUATOR, DEVICE_ID_ACTUATOR,
                              frame->cmd, 0, NULL);
            break;
        }
        
        default:
            printf("未知命令: 0x%02X\r\n", frame->cmd);
            break;
    }
}
static ProtocolFrame_t frame = {0};
void vACTUATask(void *pvParameters)
{
    
    printf("任务已启动\r\n");
	
    /* 初始化电机 */
	printf("初始化电机\r\n");
    Motor1_SetState(MOTOR_STOP);
    Motor1_SetSpeed(0);
	Motor2_SetState(MOTOR_STOP);
    Motor2_SetSpeed(0);
    
    /* 初始化舵机 */
	printf("初始化舵机\r\n");
    Servo1_SetAngle(90);   // 初始中间位置
    Servo2_SetAngle(90);
    
    /* 启动PWM输出 */
	printf("启动PWM输出\r\n");
    HAL_TIM_PWM_Start(&MOTOR1_TIM_HANDLE, MOTOR1_TIM_CHANNEL);
    HAL_TIM_PWM_Start(&MOTOR2_TIM_HANDLE, MOTOR2_TIM_CHANNEL);
    HAL_TIM_PWM_Start(&LED2_TIM_HANDLE, LED2_TIM_CHANNEL);
    HAL_TIM_PWM_Start(&SERVO1_TIM_HANDLE, SERVO1_TIM_CHANNEL);
    HAL_TIM_PWM_Start(&SERVO2_TIM_HANDLE, SERVO2_TIM_CHANNEL);
    
    /* LED初始化 */
	printf("初始化LED\r\n");
    LED1_OFF();
    LED2_SetBright(0);
    
    printf("执行器任务启动 (支持PWM电机/LED)舵机待完善..\r\n");

    
    while(1) {
        /* 等待完整帧 */
        if(ESP_UART_IsFrameReady()) {
            ESP_UART_ClearRxFlag();
            
            uint8_t* rx_buf = ESP_UART_GetRxBuffer();
            uint16_t rx_len = ESP_UART_GetRxLength();
            if(rx_buf!=NULL)
            if(Protocol_UnpackFrame(rx_buf, rx_len, &frame)) {
                Actuator_ProcessCmd(&frame);
            } else {
                printf("帧解析失败, len=%d\r\n", rx_len);
            }
        }
        
        vTaskDelay(ACTUA_TASK_DELAY);
    }
}
