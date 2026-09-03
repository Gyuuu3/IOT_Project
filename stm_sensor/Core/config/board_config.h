#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#include "main.h"

/* ========== GPIO引脚定义 ========== */

// LED
#define LED1_PORT               GPIOA
#define LED1_PIN                GPIO_PIN_1

// KEY 
#define KEY1_PORT               GPIOA
#define KEY1_PIN                GPIO_PIN_0
#define KEY_DOWN                0
#define KEY_UP                  1

// ========== 传感器引脚 ==========

// DHT11 (PA8)
#define DHT11_PORT              GPIOA
#define DHT11_PIN               GPIO_PIN_8

// 超声波 (PB12=TRIG, PB13=ECHO)
#define ULTRA_TRIG_PORT            GPIOB
#define ULTRA_TRIG_PIN             GPIO_PIN_12
#define ULTRA_ECHO_PORT            GPIOB
#define ULTRA_ECHO_PIN             GPIO_PIN_13

// ========== ADC通道映射 ==========
// PA4  - ADC1_CH4
// PA5  - ADC1_CH5
// PA6  - ADC1_CH6
// PA7  - ADC1_CH7
// PB0  - ADC1_CH8
// PB1  - ADC1_CH9

#define ADC_CHANNEL_COUNT       6
#define ADC_DMA_BUFFER_SIZE     ADC_CHANNEL_COUNT

// ADC通道映射表 (按顺序)
#define ADC_CH4_GPIO            GPIOA, GPIO_PIN_4
#define ADC_CH5_GPIO            GPIOA, GPIO_PIN_5
#define ADC_CH6_GPIO            GPIOA, GPIO_PIN_6
#define ADC_CH7_GPIO            GPIOA, GPIO_PIN_7
#define ADC_CH8_GPIO            GPIOB, GPIO_PIN_0
#define ADC_CH9_GPIO            GPIOB, GPIO_PIN_1

/* ========== 串口配置 ========== */
#define DEBUG_UART_HANDLE       huart1      // 调试日志

/* ESP32通信串口 UART2 */
#define ESP_UART_HANDLE         huart2
#define ESP_UART_BAUD           115200

/* ========== 通信帧协议 ========== */
#define FRAME_HEAD              0xAA
#define FRAME_TAIL              0xBB
#define FRAME_MAX_SIZE          128

/* ========== 设备类型 ========== */
#define DEVICE_TYPE_ACTUATOR    0x01
#define DEVICE_TYPE_SENSOR      0x02

/* ========== 传感器命令码 ========== */
#define SENSOR_CMD_GET_TEMP     0x10
#define SENSOR_CMD_GET_HUMI     0x11
#define SENSOR_CMD_GET_DIST     0x12
#define SENSOR_CMD_GET_ALL      0x13
#define SENSOR_CMD_GET_ADC      0x14
#define SENSOR_CMD_GET_ADC_CH   0x15

/* ========== 设备ID ========== */
#define DEVICE_ID_SENSOR        1

#endif
