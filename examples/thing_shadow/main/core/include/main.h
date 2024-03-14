#ifndef MAIN_H_
#define MAIN_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "stdbool.h"

extern bool wifi_sta;

typedef struct {
        char version[32];
        uint16_t publish_interval;
        char facility_id[32];
        uint8_t led;
        uint8_t dbg_mode;
        uint8_t reset;
} device_config_t;

extern device_config_t device_config;

extern bool wifi_sta;

#define Sleep(X) vTaskDelay(X * 1000 / portTICK_PERIOD_MS);

extern bool feed_watchdog;

#endif // MAIN_H