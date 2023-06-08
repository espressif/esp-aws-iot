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

void buzzer_play_james_bond()
{
    // Define the duty cycle values for the James Bond theme
    uint8_t duty_cycles[] = {128, 200, 128, 0, 128, 200, 128, 0, 128, 128, 128, 0, 128, 64, 0, 192};

    // Configure the LEDC timer
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = BUZZER_RESOLUTION,
        .freq_hz = BUZZER_FREQ_HZ,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0
    };
    ledc_timer_config(&ledc_timer);

    // Configure the LEDC channel
    ledc_channel_config_t ledc_channel = {
        .channel = BUZZER_CHANNEL,
        .duty = 0,
        .gpio_num = BUZZER_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0
    };
    ledc_channel_config(&ledc_channel);

    // Play the James Bond theme
    for (int i = 0; i < sizeof(duty_cycles) / sizeof(duty_cycles[0]); i++)
    {
        ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, duty_cycles[i]);
        ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
        vTaskDelay(pdMS_TO_TICKS(200));
    }

    buzzer_stop();
}
void buzzer_play_error_tune()
{
    // Define the duty cycle values for the error tune
    uint8_t duty_cycles[] = {200, 0, 128, 0, 200, 0, 128, 0, 200, 0};

    // Configure the LEDC timer
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = BUZZER_RESOLUTION,
        .freq_hz = BUZZER_FREQ_HZ,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0
    };
    ledc_timer_config(&ledc_timer);

    // Configure the LEDC channel
    ledc_channel_config_t ledc_channel = {
        .channel = BUZZER_CHANNEL,
        .duty = 0,
        .gpio_num = BUZZER_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0
    };
    ledc_channel_config(&ledc_channel);

    // Play the error tune
    for (int i = 0; i < sizeof(duty_cycles) / sizeof(duty_cycles[0]); i++)
    {
        ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, duty_cycles[i]);
        ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
        vTaskDelay(pdMS_TO_TICKS(20));
    }

    buzzer_stop();
}

void buzzer_play_mario_tune()
{
    // Define the duty cycle values for the Mario tune
    uint8_t duty_cycles[] = {150, 0, 150, 0, 150, 0, 100, 0, 150, 0, 100, 0, 150, 0, 0, 0};

    // Configure the LEDC timer
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = BUZZER_RESOLUTION,
        .freq_hz = BUZZER_FREQ_HZ,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0
    };
    ledc_timer_config(&ledc_timer);

    // Configure the LEDC channel
    ledc_channel_config_t ledc_channel = {
        .channel = BUZZER_CHANNEL,
        .duty = 0,
        .gpio_num = BUZZER_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0
    };
    ledc_channel_config(&ledc_channel);

    // Play the Mario tune
    for (int i = 0; i < sizeof(duty_cycles) / sizeof(duty_cycles[0]); i++)
    {
        ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, duty_cycles[i]);
        ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
        vTaskDelay(pdMS_TO_TICKS(200));
    }

    buzzer_stop();
}

void buzzer_play_happy_tune()
{
    // Define the duty cycle values for the happy tune
    uint8_t duty_cycles[] = {128, 150, 170, 192, 170, 150, 128, 128};

    // Configure the LEDC timer
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = BUZZER_RESOLUTION,
        .freq_hz = BUZZER_FREQ_HZ,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0
    };
    ledc_timer_config(&ledc_timer);

    // Configure the LEDC channel
    ledc_channel_config_t ledc_channel = {
        .channel = BUZZER_CHANNEL,
        .duty = 0,
        .gpio_num = BUZZER_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0
    };
    ledc_channel_config(&ledc_channel);

    // Play the happy tune
    for (int i = 0; i < sizeof(duty_cycles) / sizeof(duty_cycles[0]); i++)
    {
        ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, duty_cycles[i]);
        ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
        vTaskDelay(pdMS_TO_TICKS(300));
    }

    buzzer_stop();
}
