
#include "esp_attr.h"
#include "esp_sleep.h"
#include <sys/time.h>
#include <time.h>
// #include "protocol_examples_common.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_sntp.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "main.h"
#include "nvs_flash.h"
#include <string.h>

#include "driver/gpio.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "wifi_driver/data_store_nvs.h"
#include "wifi_driver/http_server.h"
#include "wifi_driver/wifi_manager.h"

// #include "parse/Cjsonoperation.h"


#define ESP_WIFI_SSIDAP "aws_device"
#define ESP_WIFI_PASSAP "aws123456789"
#define ESP_WIFI_CHANNEL 1
#define MAX_STA_CONN 4
#define MAXIMUM_RETRY 3
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1
#define RESETPIN 2

int disconnectivity_count = 0;
/* FreeRTOS event group to signal when we are connected*/
wifi_mode_t mode;
static EventGroupHandle_t s_wifi_event_group;
uint8_t wifi_state = false;
bool wifi_sta = false;
/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */

static const char *TAG = "wifi station";
static int s_retry_num = 0;

void Wifi_STA(void) {
        wifi_config_t wifi_config_sta = {
        .sta = {
            //  .ssid = SSID,
            //  .password = PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false},
        },
    };

        sprintf((char *) wifi_config_sta.sta.ssid, "%s", ssid);
        sprintf((char *) wifi_config_sta.sta.password, "%s", pswd);
        printf("SSID =%s    &&&& Pass=%s\n", ssid, pswd);
        wifi_mode_t mode;
        if (esp_wifi_get_mode(&mode) == ESP_OK) {
                if (mode != WIFI_MODE_APSTA) {
                        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
                }
        }
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config_sta));

        ESP_LOGI(TAG, "wifi_init_sta finished.");
}

void Wifi_AP(void) {
        wifi_config_t wifi_config_ap = {
                .ap = { .ssid = ESP_WIFI_SSIDAP,
                        .channel = 1,
                        .ssid_len = strlen(ESP_WIFI_SSIDAP),
                        .password = ESP_WIFI_PASSAP,
                        .max_connection = MAX_STA_CONN,
                        .authmode = WIFI_AUTH_WPA_WPA2_PSK },
        };

        if (strlen(ESP_WIFI_PASSAP) == 0) {
                wifi_config_ap.ap.authmode = WIFI_AUTH_OPEN;
        }
        esp_wifi_set_mode(WIFI_MODE_AP);
        esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config_ap);
        ESP_LOGI(TAG, "wifi_init_ap finished.");
}

void Wifi_AP_STA(void) {
        Wifi_STA();
        Wifi_AP();
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
}

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
        if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
                ESP_LOGI(TAG, "starting sta...");
                esp_wifi_connect();
        }

        if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {

                if (esp_wifi_get_mode(&mode) == ESP_OK) {
                        if (s_retry_num >= MAXIMUM_RETRY && mode != WIFI_MODE_APSTA) {
                                ESP_LOGI(TAG, "Mode changed to AP+STA");

                                ESP_ERROR_CHECK(esp_wifi_stop());
                                ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
                                ESP_ERROR_CHECK(esp_wifi_start());
                                esp_wifi_set_max_tx_power(34);
                        }
                }

                ESP_LOGI(TAG, "Reconnecting...");
                esp_wifi_connect();
                s_retry_num++;

                disconnectivity_count++; // when disconnected for longer duration then relogin it is used in
                                         // network task manager to help detect relogin
                if (disconnectivity_count > 1000) {
                        disconnectivity_count = 30;
                }
        }
        if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED) {
                s_retry_num = 0;

                if (esp_wifi_get_mode(&mode) == ESP_OK) {
                        if (mode == WIFI_MODE_APSTA) {
                                ESP_LOGI(TAG, "Mode changed to STA");

                                ESP_ERROR_CHECK(esp_wifi_stop());
                                ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
                                ESP_ERROR_CHECK(esp_wifi_start());
                                esp_wifi_set_max_tx_power(34);
                        }
                }
        }

        if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
                ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
                ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
                s_retry_num = 0;
                xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        }

        switch (event_id) {
        case WIFI_EVENT_STA_CONNECTED: {
                wifi_sta = true;
                wifi_state = WIFI_EVENT_STA_CONNECTED;
        } break;
        case WIFI_EVENT_STA_START: {
                wifi_state = WIFI_EVENT_STA_START;
        } break;
        case WIFI_EVENT_STA_DISCONNECTED: {
                wifi_sta = false;
                wifi_state = WIFI_EVENT_STA_DISCONNECTED;
        } break;
        case WIFI_EVENT_AP_STADISCONNECTED: {
        } break;
        default:
                break;
        }
}

void wifi_init_sta(void) {
        s_wifi_event_group = xEventGroupCreate();

        ESP_ERROR_CHECK(esp_netif_init());

        ESP_ERROR_CHECK(esp_event_loop_create_default());
        esp_netif_t *ap_netif = esp_netif_create_default_wifi_ap();
        assert(ap_netif);
        esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
        assert(sta_netif);

        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));

        esp_event_handler_instance_t instance_any_id;
        esp_event_handler_instance_t instance_got_ip;
        ESP_ERROR_CHECK(esp_event_handler_instance_register(
                        WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id));
        ESP_ERROR_CHECK(esp_event_handler_instance_register(
                        IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip));

        WebServerEventRegister();

        GetWifiCred(); // load ssid and pswd from nvs
        Wifi_AP_STA();
        ESP_ERROR_CHECK(esp_wifi_start());
        esp_wifi_set_max_tx_power(34);

        ESP_LOGI(TAG, "wifi_init_sta finished.");

        /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for
         * the maximum number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
        EventBits_t bits = xEventGroupWaitBits(
                        s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

        /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event
         * actually happened. */
        if (bits & WIFI_CONNECTED_BIT) {
                // ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                //          SSID, PASS);
        } else if (bits & WIFI_FAIL_BIT) {
                // ESP_LOGE(TAG, "Failed to connect to SSID:%s, password:%s",
                //          SSID, PASS);
        } else {
                ESP_LOGE(TAG, "UNEXPECTED EVENT");
        }
}

void NvsResetTask() {
        gpio_reset_pin(RESETPIN);
        gpio_set_direction(RESETPIN, GPIO_MODE_INPUT);
        gpio_set_pull_mode(RESETPIN, GPIO_PULLUP_ONLY);

        while (1) {
                if (gpio_get_level(RESETPIN)) {
                        vTaskDelay(4900 / portTICK_RATE_MS);
                        if (gpio_get_level(RESETPIN)) {
                                nvs_flash_erase();
                                esp_restart();
                        } else {
                        }
                }
                vTaskDelay(100 / portTICK_RATE_MS);
        }
}
void WifiInit() {
        // NVSInit();
        esp_log_level_set(TAG, ESP_LOG_INFO); // Set UART log level
        esp_err_t ret = nvs_flash_init();
        if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
                ESP_ERROR_CHECK(nvs_flash_erase());
                ret = nvs_flash_init();
        }

        ESP_ERROR_CHECK(ret);
        ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
        wifi_init_sta();
        //xTaskCreate(NvsResetTask, "NVS reset", 1024 * 2, NULL, 2, NULL);
}