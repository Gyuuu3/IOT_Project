#include "gpio_led.h"
#include "board_config.h"
#include "stdbool.h"
void LED_Init(void)
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE; // 禁用中断
    io_conf.mode = GPIO_MODE_OUTPUT;       // 设置为输出模式
    io_conf.pin_bit_mask = (1ULL << LED_GPIO_PIN); // 配置LED引脚
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;  // 禁用下拉
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;      // 禁用上拉
    gpio_config(&io_conf);
    gpio_set_level(LED_GPIO_PIN, 0); // 初始化LED为关闭状态
}

void LED_Set_State(bool state)
{
    gpio_set_level(LED_GPIO_PIN, state);
}

