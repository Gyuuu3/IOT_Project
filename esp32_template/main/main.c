#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "system_init.h"
void app_main(void)
{

    System_Hardware_Init();

    RTOS_Create_ALL_Tasks();
    
}
