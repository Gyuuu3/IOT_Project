#include "task_wifi.h"
#include "stdbool.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "board_config.h"
#include "gpio_led.h"
#include "debug_print.h"
#include "task_config.h"

/* 存疑 */
static bool wifi_connected = false;

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data)
{
    static uint8_t retry_count = 0;

    if(event_base == WIFI_EVENT){
        switch(event_id){

            case WIFI_EVENT_STA_START:
                esp_wifi_connect();
                break;

            case WIFI_EVENT_STA_DISCONNECTED:
                wifi_connected = false;
                Debug_Print("WiFi断开,重连次数[%d]...\n",++retry_count);
                LED_Set_State(0);
                esp_wifi_connect();
                
                break;

            default:
                break;
        }

    }
    else if(event_base == IP_EVENT){
        switch(event_id){
            case IP_EVENT_STA_GOT_IP:
                wifi_connected = true;
                Debug_Print("WiFi已连接,获取IP成功\n");
                retry_count = 0;
                LED_Set_State(1);
                break;
            default:
                break;
        }

    }

}

void vWIFI_Task(void *pvParameters)
{
    // 初始化NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // 初始化TCP/IP
    ESP_ERROR_CHECK(esp_netif_init());

    // 创建默认的事件循环
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // 创建默认的WiFi站点接口
    esp_netif_create_default_wifi_sta();

    // 初始化WiFi配置
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // 注册WiFi事件处理程序
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));

    // 配置WiFi连接参数
    static wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    // 设置WiFi模式为STA（Station）
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    
    // 启动WiFi
    ESP_ERROR_CHECK(esp_wifi_start());

    Debug_Print("WiFi任务已启动，正在连接到WiFi...\n");

   
    static bool last_state = false;
    while (1) {
        vTaskDelay(WIFI_TASK_DELAY);
        /* 只在状态变化时打印 */
        if (wifi_connected != last_state) {
            last_state = wifi_connected;
            Debug_Print(wifi_connected ? "WiFi已连接\n" : "WiFi已断开\n");
        }
    }

}

