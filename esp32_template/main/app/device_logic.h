#ifndef __DEVICE_LOGIC_H__
#define __DEVICE_LOGIC_H__
#include "task_handle.h"
#include "mqtt_client.h"
#include "stdbool.h"

void Device_ParseMQTTCmd(uint8_t *data, uint16_t length);
void Device_PublishData(const char *data);
void Device_ProcessSensorData(ProtocolFrame_t *frame);
bool Device_SendToActuator(uint8_t device_id, uint8_t cmd, uint8_t *data, uint8_t data_len);
bool Device_SendToSensor(uint8_t device_id, uint8_t cmd, uint8_t *data, uint8_t data_len);

#endif