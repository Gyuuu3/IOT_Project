#ifndef __TASK_MQTT_H__
#define __TASK_MQTT_H__
#include "mqtt_client.h"

esp_mqtt_client_handle_t mqtt_get_client(void);
void vMQTT_Task(void* pvParamter);
#endif
