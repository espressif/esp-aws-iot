#ifndef DEMO_CONFIG_H_
#define DEMO_CONFIG_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "freertos/queue.h"
#include "sdkconfig.h"
//#include "protocol_examples_common.h"
#include "driver/gpio.h"
#include "wifi_driver/wifi_manager.h"
#include "main.h"
#include "sleep.h"
#include "esp_task_wdt.h"
#include "ping_time.h"

#define PUBLISH_TIME_S 5 
#define TWDT_TIMEOUT_S PUBLISH_TIME_S*3
#define PANIC_ENABLE 1
#define WATCHDOG_FEED_TIME 5 //time in sec

#endif




