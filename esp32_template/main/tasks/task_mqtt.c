#include "task_mqtt.h"
#include "board_config.h"
#include "debug_print.h"
#include "task_handle.h"
#include "device_logic.h"
#include "task_config.h"

esp_mqtt_client_handle_t mqtt_client;

esp_mqtt_client_handle_t mqtt_get_client(void){
    return mqtt_client;
}

static void mqtt_event_cb(void* handler_args,esp_event_base_t base,
                        int32_t event_id,void* event_data) {
    esp_mqtt_event_handle_t event = event_data;

    switch(event_id){
        case MQTT_EVENT_CONNECTED:
            esp_mqtt_client_subscribe(mqtt_client,MQTT_SUBSCRIBE_TOPIC,0);
            esp_mqtt_client_subscribe(mqtt_client,MQTT_TEST_TOPIC,0);
            Debug_Print("MQTT连接成功，已订阅主题%s\r\n",MQTT_SUBSCRIBE_TOPIC);
            Debug_Print("MQTT连接成功，已订阅主题%s\r\n",MQTT_TEST_TOPIC);
            Device_PublishData("are you ok?");
            break;

        case MQTT_EVENT_DISCONNECTED:
            Debug_Print("MQTT断开连接\r\n");
            break;

        case MQTT_EVENT_DATA:
            Debug_Print("收到主题:%.*s的消息\n",event->topic_len,event->topic);
            
            MsgData_t msg = {0};
            msg.length = event->data_len < sizeof(msg.data) ? event->data_len : sizeof(msg.data) - 1;
            memcpy(msg.data,event->data,msg.length);
            msg.data[msg.length] = '\0';
            Device_ParseMQTTCmd(msg.data,msg.length);
            break;

        case MQTT_EVENT_ERROR:
            Debug_Print("MQTT错误\r\n");
            break;

        default:
            break;
    }
}

void vMQTT_Task(void* pvParamter){
    
    static esp_mqtt_client_config_t mqtt_config={
        .broker.address.uri = MQTT_BROKER_URI,
        .credentials.client_id = MQTT_CLIENT_ID,
        .credentials.username = MQTT_USER_NAME,
        .credentials.authentication.password =MQTT_PASSWORD,
        
    };
    mqtt_client = esp_mqtt_client_init(&mqtt_config);

    /* 注册事件 */
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_cb, NULL);
    /* 启动mqtt */
    esp_mqtt_client_start(mqtt_client);

    /* 测试消息发布 */
    Debug_Print("MQTT任务已启动，正在连接到mqtt...\n");
    while(1){

        vTaskDelay(MQTT_TASK_DELAY);

    }

}
