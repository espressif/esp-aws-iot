/* thing-shadow example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "core_utility.h"
#include "ldo_control.h"
#include "buzzer.h"
#include "uart_handler.h"
#include "sensors.h"

int aws_shadow_main(int argc, char **argv);

#include "esp_log.h"

device_config_t device_config = {0x00};

bool feed_watchdog = true;

static const char *TAG = "CLASSIC-SHADOW";

/**
 * @brief
 *
 * @param param
 */
void feedWatchDog(void *param)
{
    esp_task_wdt_init(TWDT_TIMEOUT_S, PANIC_ENABLE);
    ESP_ERROR_CHECK(esp_task_wdt_add(NULL));
    ESP_ERROR_CHECK(esp_task_wdt_status(NULL));
    while (1)
    {
        if (feed_watchdog)
        {
            esp_task_wdt_reset();
        }
        vTaskDelay(WATCHDOG_FEED_TIME * 1000 / portTICK_RATE_MS); // feeding wdt
    }
}
/**
 * @brief
 *
 */
static void WatchdogInit()
{

    xTaskCreate(feedWatchDog, "SleepTask", 1024 * 2, NULL, 2, NULL);
}

/**
 * @brief
 *
 */
void core_start(void)
{
    Sleep(5);
    ldo_init();
    ldo_on();
    // buzzer_play_james_bond();
    // setting watchdog timer for 2 seconds
    WatchdogInit(); // Initializing Watchdog timer

    WifiInit(); // WiFi Initialization

    read_nvs_config(NULL);

    GetStandardTime(); // Get Standard time
    uart_initialize();


}

/*
 * Prototypes for the demos that can be started from this project.  Note the
 * Shadow demo is not actually started until the network is already.
 */

void app_main()
{
    //** start the core functionality
    core_start();
    //** classsic shadow
    aws_shadow_main(0, NULL);
}
