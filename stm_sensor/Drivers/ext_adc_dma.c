#include "ext_adc_dma.h"
#include "FreeRTOS.h"
#include "task.h"

// ADC DMA 缓冲区
static uint16_t adc_dma_buffer[ADC_CHANNEL_COUNT];
static uint8_t adc_dma_complete = 0;

void ADC_DMA_Start(void){
	//启动ADC DMA连续转换
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc_dma_buffer,ADC_CHANNEL_COUNT);
}

void ADC_DMA_Stop(void){
	HAL_ADC_Stop_DMA(&hadc1);
}

// DMA传输完成完成回调
void HAL_ADC_ConCpltCallback(ADC_HandleTypeDef* hadc){
	if(hadc->Instance==ADC1)
		adc_dma_complete=1;
}

bool ADC_DMA_Read(ADC_Data_t* data){
	if(data==NULL)	return false;
	if(!adc_dma_complete)	return false;
	
	adc_dma_complete=0;
	
	// 复制数据
	for(int i=0;i<ADC_CHANNEL_COUNT;i++){
		data->raw_values[i]=adc_dma_buffer[i];
		 
		// 3.3v/4096
		data->voltages[i]=(float)adc_dma_buffer[i]*3.3f/4096;
	}
	
	data->valid =true;
	data->last_read_time =xTaskGetTickCount();
	
	return true;

}

uint16_t ADC_GetValue(uint8_t channel){
	if(channel>=ADC_CHANNEL_COUNT)	return 0;
	return adc_dma_buffer[channel];
}

float ADC_GetVoltage(uint8_t channel){
	if(channel>=ADC_CHANNEL_COUNT)	return 0;
	return adc_dma_buffer[channel]*3.3f/4096;
}





