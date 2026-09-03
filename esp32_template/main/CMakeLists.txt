idf_component_register(
    SRCS "main.c"
         "system_init.c"
         "tasks/task_wifi.c"
         "tasks/task_mqtt.c"
         "tasks/task_sensor.c"
         "tasks/task_actuator.c"
         "tasks/task_debug.c"
         "tasks/task_test.c"
         "app/device_logic.c"               
    INCLUDE_DIRS "." "./tasks" "./app" 
    REQUIRES config drivers middleware driver esp_wifi mqtt freertos nvs_flash
)

