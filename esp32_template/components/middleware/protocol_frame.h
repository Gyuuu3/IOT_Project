#ifndef __PROTOCOL_FRAME_H__
#define __PROTOCOL_FRAME_H__
#include "task_handle.h"
#include "stdbool.h"

// 执行器命令
#define CMD_LED_ON      0x01
#define CMD_LED_OFF     0x02
#define CMD_LED_TOGGLE  0x03
#define CMD_MOTOR_RUN   0x04
#define CMD_MOTOR_STOP  0x05
#define CMD_MOTOR_REV   0x06
#define CMD_SERVO_FWD   0x07
#define CMD_SERVO_REV   0x08
#define CMD_SERVO_RST   0x09

// 传感器命令
#define CMD_GET_TEMP    0x10
#define CMD_GET_HUMID   0x11
#define CMD_GET_DIST    0x12
#define CMD_GET_ALL     0x13

void Protocol_PackFrame(uint8_t device_type, uint8_t device_id, 
                        uint8_t cmd, uint8_t *data, uint8_t data_len,
                        MsgData_t *out_msg);

bool Protocol_UnpackFrame(uint8_t *data, uint16_t length, 
                          ProtocolFrame_t *out_frame);

void Protocol_PrintFrame(ProtocolFrame_t *frame);

#endif