#ifndef __EXT_DHT11_H__
#define __EXT_DHT11_H__
#include "stdint.h"
#include "stdbool.h"

typedef struct {
	float temperature;
	float humidity;
	bool valid;
	uint32_t last_read_time;
} DHT11_Data_t; 

bool DHT11_Read(DHT11_Data_t *data);
float DHT11_GetTemp(void);
float DHT11_GetHumi(void);


#endif
