#ifndef BUZZER_H
#define BUZZER_H

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BUZZER_GPIO GPIO_NUM_0
#define BUZZER_CHANNEL LEDC_CHANNEL_0
#define BUZZER_FREQ_HZ 2000
#define BUZZER_RESOLUTION LEDC_TIMER_8_BIT

void buzzer_stop();
void buzzer_play_tone();
void buzzer_play_heartbeat();
void buzzer_play_james_bond();
void buzzer_play_error_tune();
void buzzer_play_mario_tune();
void buzzer_play_happy_tune();

#endif // BUZZER_H
