#include <stdio.h>
#include <stdlib.h>
#include "sensors.h"
#include "driver/temp_sensor.h"
#include "esp_log.h"

#define INT_STRING_SIZE 10 // Adjust the size as per your requirement

static const char *TAG = "SENSORS";

void sensors_initialize()
{
    ESP_LOGI(TAG, "Initializing Temperature sensor");
    temp_sensor_config_t temp_sensor = TSENS_CONFIG_DEFAULT();
    temp_sensor_get_config(&temp_sensor);
    ESP_LOGI(TAG, "default dac %d, clk_div %d", temp_sensor.dac_offset, temp_sensor.clk_div);
    temp_sensor.dac_offset = TSENS_DAC_DEFAULT; // DEFAULT: range:-10℃ ~  80℃, error < 1℃.
    temp_sensor_set_config(temp_sensor);
    temp_sensor_start();
    ESP_LOGI(TAG, "Temperature sensor started");
}

static char* int_to_string(int value)
{
    static char str[INT_STRING_SIZE];
    snprintf(str, INT_STRING_SIZE, "%d", value);
    return str;
}

char* get_board_temp()
{
    char *out_temp = "nan";
    float tsens_out;
    if (temp_sensor_read_celsius(&tsens_out) != ESP_OK) {
        ESP_LOGE(TAG, "Error reading temperature data");
        return out_temp;
    }

    int temperature = (int)tsens_out; // Convert float to int
    ESP_LOGI(TAG, "Temperature out celsius %d°C", temperature);
    out_temp = int_to_string(temperature);
    return out_temp;
}