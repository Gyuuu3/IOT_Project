#ifndef __EXT_ADC_DMA_H__
#define __EXT_ADC_DMA_H__
#include "stdint.h"
#include "stdbool.h"
#include "board_config.h"
#include "adc.h"
typedef struct {
	uint16_t raw_values[ADC_CHANNEL_COUNT];
	float voltages[ADC_CHANNEL_COUNT];
	bool valid;
	uint32_t last_read_time;

} ADC_Data_t;

void ADC_DMA_Start(void);
void ADC_DMA_Stop(void);
void HAL_ADC_ConCpltCallback(ADC_HandleTypeDef* hadc);
bool ADC_DMA_Read(ADC_Data_t* data);
uint16_t ADC_GetValue(uint8_t channel);
float ADC_GetVoltage(uint8_t channel);
#endif
