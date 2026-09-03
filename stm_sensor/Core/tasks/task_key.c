#include "task_key.h"
#include "board_config.h"
#include "task_config.h"
#include "usart.h"
#include "ext_gpio_key.h"
#include "ext_uart_esp.h"
#include "protocol_frame.h"

void vKeyTask(void* pvParameters){
    uint8_t key=0;
    uint8_t last_key = 0;
    printf("按键检测任务启动\r\n");
    printf("  按键按下将发送协议帧到ESP32\r\n");
    
    while(1){
        key = Key_Scan();
        
        if(key == 1 && last_key == 0){
            printf("按键1按下，发送协议帧到ESP32\r\n");
            
            // 发送有效的协议帧: 传感器设备上报按键事件
            // 帧格式: [AA][TYPE=0x02][ID=1][CMD=0x20][LEN=1][DATA=0x01][BB]
            // 表示: 传感器ID=1，命令0x20(按键事件)，数据0x01(按键1)
            uint8_t key_data[] = {0x01};  // 按键ID=1
            ESP_UART_SendFrame(DEVICE_TYPE_SENSOR, DEVICE_ID_SENSOR, 0x20, 1, key_data);
            
            printf("协议帧已发送: AA 02 01 20 01 01 BB\r\n");
        }
        
        last_key = key;
        vTaskDelay(KEY_TASK_DELAY);
    }
}