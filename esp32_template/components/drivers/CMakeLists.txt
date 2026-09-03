idf_component_register(
    SRCS "gpio_led.c"
         "gpio_key.c"
         "uart_actuator.c" 
         "uart_sensor.c"
        
    INCLUDE_DIRS "."
    REQUIRES driver config middleware
)