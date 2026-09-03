// task_esp.c
#include "task_esp.h"
#include "task_config.h"
#include "board_config.h"
#include "usart.h"
#include "ext_uart_esp.h"

/* 引用UART2 DMA缓冲 */
extern uint8_t uart2_rx_buf[];
extern volatile uint16_t uart2_rx_len;
extern volatile uint8_t uart2_frame_ready;


void vESPTask(void* pvParameters)
{
    printf("STM<->ESP 通信任务启动(DMA+IDLE模式)\r\n");
    printf("  协议: [AA][TYPE][ID][CMD][LEN][DATA...][BB]\r\n");
    
    while(1) {
        /* 检查是否有数据帧到达（由UART空闲中断触发） */
        if(uart2_frame_ready) {
            //uart2_frame_ready = 0;
            
            /* 数据转发给执行器任务处理（通过全局变量） */
            /* 执行器任务会从 uart2_rx_buf 读取数据 */
            /* 调试打印 */
            printf("RX(%d): ", uart2_rx_len);
            for(int i=0; i<uart2_rx_len; i++) {
                printf("%02X ", uart2_rx_buf[i]);
            }
            printf("\r\n");
        }
        
        vTaskDelay(ESP_TASK_DELAY);
    }
}
