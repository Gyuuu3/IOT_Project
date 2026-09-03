#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#include "driver/gpio.h"
/* 硬件引脚配置 */

/* LED */
#define LED_GPIO_PIN  GPIO_NUM_4

/* KEY */
#define KEY1_GPIO_PIN GPIO_NUM_18
#define KEY2_GPIO_PIN GPIO_NUM_11
#define KEY3_GPIO_PIN GPIO_NUM_13



/* UART1 ESP32 <-> STM32执行器 */
#define UART_ACTUA_NUM  UART_NUM_1
#define UART_ACTUA_TXD  GPIO_NUM_16
#define UART_ACTUA_RXD  GPIO_NUM_15
#define UART_ACTUA_BAUD 115200


/* UART2 ESP32 <-> STM32传感器 */
#define UART_SENSOR_NUM UART_NUM_2
#define UART_SENSOR_TXD GPIO_NUM_20
#define UART_SENSOR_RXD GPIO_NUM_19
#define UART_SENSOR_BAUD 115200

/* WiFi配置 */
#define WIFI_SSID "11111"
#define WIFI_PASSWORD "22224444"
#define WIFI_MAX_ENTRY 5

/* MQTT配置 */
#define MQTT_BROKER_URI "mqtt://75d1694475.st1.iotda-device.cn-east-3.myhuaweicloud.com:1883"
#define MQTT_CLIENT_ID "6a5f145718855b39c53282db_esp32_0_0_2026072208"
#define MQTT_USER_NAME "6a5f145718855b39c53282db_esp32"
#define MQTT_PASSWORD   "68ae24c4d953751a9d8cfe8523f197d5867deb019b7d55870e03f6fe9fe5c150"
#define MQTT_SUBSCRIBE_TOPIC "device/cmd"
#define MQTT_PUBLISH_TOPIC "device/report"
#define MQTT_TEST_TOPIC "device/test"

/* 通信协议帧头尾 */
#define FRAME_HEADER 0xAA
#define FRAME_TAIL 0xBB
#define FRAME_MAX_SIZE 128

/* 环形缓冲区配置 */
#define RING_BUFFER_SIZE        4096    

/* 设备类型 */
#define DEVICE_TYPE_ACTUATOR  0x01  // 执行器STM32
#define DEVICE_TYPE_SENSOR    0x02  // 传感器STM32

/* 执行器命令码 */
#define ACTUATOR_CMD_LED_ON     0x01
#define ACTUATOR_CMD_LED_OFF    0x02
#define ACTUATOR_CMD_LED_TOGGLE 0x03
#define ACTUATOR_CMD_MOTOR_RUN  0x04
#define ACTUATOR_CMD_MOTOR_STOP 0x05
#define ACTUATOR_CMD_MOTOR_REV  0x06
#define ACTUATOR_CMD_SERVO_FWD  0x07
#define ACTUATOR_CMD_SERVO_REV  0x08
#define ACTUATOR_CMD_SERVO_RST  0x09


/* 传感器命令码 */
#define SENSOR_CMD_GET_TEMP     0x10
#define SENSOR_CMD_GET_HUMID    0x11
#define SENSOR_CMD_GET_DISTANCE 0x12
#define SENSOR_CMD_GET_ALL      0x13



#endif