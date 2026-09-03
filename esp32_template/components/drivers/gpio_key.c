#include "gpio_key.h"
#include "board_config.h"
#include "driver/gpio.h"

void KEY_Init(void){
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << KEY1_GPIO_PIN) | 
                        (1ULL << KEY2_GPIO_PIN) | 
                        (1ULL << KEY3_GPIO_PIN),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE,  
    };
    gpio_config(&io_conf);
}

bool KEY1_GetState(void){
    return gpio_get_level(KEY1_GPIO_PIN) == 0;  // 按下为低电平
}

bool KEY2_GetState(void){
    return gpio_get_level(KEY2_GPIO_PIN) == 0;
}

bool KEY3_GetState(void){
    return gpio_get_level(KEY3_GPIO_PIN) == 0;
}