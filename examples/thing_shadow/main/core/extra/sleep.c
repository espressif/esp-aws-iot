
#include "sleep.h"
#include "cJSON.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "main.h"
#include "nvs_platform.h"
#include "ping_time.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POWER_PIN 36

#define WRITE 1
#define READ 0

/**
 * @brief
 *
 */
void write_nvs_config() {
        nvs_perform_str("version", device_config.version, STR, WRITE);
        nvs_perform_str("customerid", device_config.customer_id, STR, WRITE);
        nvs_perform_number("pub_inter", &device_config.publish_interval, U16, WRITE);
        nvs_perform_number("led", &device_config.led, U8, WRITE);
        nvs_perform_number("dgb_mode", &device_config.dbg_mode, U8, WRITE);
}
/**
 * @brief
 *
 */
void read_nvs_config() {
    
        nvs_perform_str("version", device_config.version, STR, READ);
        nvs_perform_str("customerid", device_config.customer_id, STR, READ);
        nvs_perform_number("pub_inter", &device_config.publish_interval, U16, READ);
        nvs_perform_number("led", &device_config.led, U8, READ);
        nvs_perform_number("dgb_mode", &device_config.dbg_mode, U8, READ);

        //** fall back condition

        device_config.publish_interval = (device_config.publish_interval == 0) ?
                        30 :
                        device_config.publish_interval;

        if (strlen(device_config.version) == 0)
                sprintf(device_config.version, "%s", "v1.0.0");

        if (strlen(device_config.customer_id) == 0) {
                sprintf(device_config.customer_id, "%s", "testing");
                write_nvs_config();
        }
}
