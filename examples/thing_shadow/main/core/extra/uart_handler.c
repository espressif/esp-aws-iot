#include "uart_handler.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "soc/uart_struct.h"
#include "string.h"
#include "driver/gpio.h"

#define UART_PORT          (UART_NUM_0)
#define TEST_TXD   (21)
#define TEST_RXD   (20)
#define TEST_RTS   UART_PIN_NO_CHANGE
#define TEST_CTS   UART_PIN_NO_CHANGE

#define BUF_SIZE        (127)
#define BAUD_RATE       (38400)
#define PACKET_READ_TICS        (100 / portTICK_RATE_MS)

#define MAX_RETRY_ATTEMPTS    (5)


static const char *TAG = "RS485_Handler";

bool uart_initialize()
{
    const int uart_num = UART_PORT;
    uart_config_t uart_config = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
    };

    esp_log_level_set(TAG, ESP_LOG_INFO);
    ESP_LOGI(TAG, "Start RS485 application test and configure UART.");

    if (uart_param_config(uart_num, &uart_config) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure UART parameters");
        return false;
    }

    ESP_LOGI(TAG, "UART set pins, mode, and install driver.");
    if (uart_set_pin(uart_num, TEST_TXD, TEST_RXD, TEST_RTS, TEST_CTS) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set UART pins");
        return false;
    }

    if (uart_driver_install(uart_num, BUF_SIZE * 2, 0, 0, NULL, 0) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to install UART driver");
        return false;
    }

    if (uart_set_mode(uart_num, UART_MODE_RS485_HALF_DUPLEX) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set UART mode");
        return false;
    }

    return true;
}

bool get_uart_data(char* buffer, size_t buffer_size)
{
    uint8_t* data = (uint8_t*)malloc(BUF_SIZE);
    if (data == NULL) {
        ESP_LOGE(TAG, "Failed to allocate memory for data buffer");
        return false;
    }

    int len = 0;

    const char* test_str = "3101valuer\n\r";

    for (int ctr = 0; ctr < MAX_RETRY_ATTEMPTS; ctr++) {
        uart_flush(UART_PORT);
        uart_write_bytes(UART_PORT, test_str, strlen(test_str));
        vTaskDelay(pdMS_TO_TICKS(50));
        len = uart_read_bytes(UART_PORT, data, BUF_SIZE, PACKET_READ_TICS);
        ESP_LOGI(TAG, "data length: %d", len);

        if (len >= 25) {
            ESP_LOGI(TAG, "Received data: %s \nData looks valid. Exiting the loop...", (char*)data);
            snprintf(buffer, buffer_size, "%.*s", len, (char*)data);
            free(data);
            uart_flush(UART_PORT);
            vTaskDelay(pdMS_TO_TICKS(50));
            return true;
        } else {
            ESP_LOGI(TAG, "Invalid data: %s \nTrying again ...", (char*)data);
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }

    free(data);
    uart_flush(UART_PORT);
    return false;
}
