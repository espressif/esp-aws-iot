#include "buzzer.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdlib.h>  // For generating random numbers

#define BUZZER_GPIO GPIO_NUM_0
#define BUZZER_CHANNEL LEDC_CHANNEL_0
#define BUZZER_FREQ_HZ 2000
#define BUZZER_RESOLUTION LEDC_TIMER_8_BIT


void buzzer_stop()
{
    // Stop the buzzer
    ledc_stop(LEDC_LOW_SPEED_MODE, BUZZER_CHANNEL, 0);

    // Turn off the GPIO
    gpio_set_level(BUZZER_GPIO, 0);
}

void buzzer_play_tone()
{
    // Play the tone
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = BUZZER_RESOLUTION,
        .freq_hz = BUZZER_FREQ_HZ,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0
    };
    ledc_timer_config(&ledc_timer);
    
    ledc_channel_config_t ledc_channel = {
        .channel = BUZZER_CHANNEL,
        .duty = 128,
        .gpio_num = BUZZER_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0
    };
    ledc_channel_config(&ledc_channel);

    // Play the tone for 1 second
    vTaskDelay(pdMS_TO_TICKS(200));
    buzzer_stop();

}



void buzzer_play_heartbeat()
{
    // Play the tone resembling a single heartbeat
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = BUZZER_RESOLUTION,
        .freq_hz = BUZZER_FREQ_HZ,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .channel = BUZZER_CHANNEL,
        .duty = 128,
        .gpio_num = BUZZER_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0
    };
    ledc_channel_config(&ledc_channel);

    // Play the low-pitch beat
    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 0);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
    vTaskDelay(pdMS_TO_TICKS(50));

    // Play the higher-pitch beat
    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 128);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
    vTaskDelay(pdMS_TO_TICKS(50));
    buzzer_stop();
}

void buzzer_play_progress()
{

    // Play the tone resembling a single heartbeat
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = BUZZER_RESOLUTION,
        .freq_hz = BUZZER_FREQ_HZ,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .channel = BUZZER_CHANNEL,
        .duty = 128,
        .gpio_num = BUZZER_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0
    };
    ledc_channel_config(&ledc_channel);

    // Play the low-pitch beat
    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 0);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
    vTaskDelay(pdMS_TO_TICKS(75));

    // Play the higher-pitch beat
    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 64);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
    vTaskDelay(pdMS_TO_TICKS(75));

    // Play the higher-pitch beat
    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 128);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
    vTaskDelay(pdMS_TO_TICKS(75));

    // Play the higher-pitch beat
    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 192);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
    vTaskDelay(pdMS_TO_TICKS(75));
    buzzer_stop();
}