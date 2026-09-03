#ifndef __GPIO_KEY_H__
#define __GPIO_KEY_H__
#include "stdbool.h"

void KEY_Init(void);
bool KEY1_GetState(void);
bool KEY2_GetState(void);
bool KEY3_GetState(void);

#endif