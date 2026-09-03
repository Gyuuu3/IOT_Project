#ifndef __EXT_ULTRASONIC_H__
#define __EXT_ULTRASONIC_H__
#include "stdint.h"
#include "stdbool.h"

typedef struct {
	float distance_cm;
	bool valid;
	uint32_t last_read_time;

} Ultra_Data_t;

bool Ultra_Read(Ultra_Data_t *data);
float Ultra_GetDistance(void);


#endif