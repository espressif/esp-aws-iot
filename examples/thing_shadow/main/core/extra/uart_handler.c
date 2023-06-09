/* Uart Events Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

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
#define TASK_STACK_SIZE    (2048)
#define TASK_PRIO          (10)

static const char *TAG = "RS485_Handler";

void uart_initialize()
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

    uart_param_config(uart_num, &uart_config);
    ESP_LOGI(TAG, "UART set pins, mode, and install driver.");
    uart_set_pin(uart_num, TEST_TXD, TEST_RXD, TEST_RTS, TEST_CTS);
    uart_driver_install(uart_num, BUF_SIZE * 2, 0, 0, NULL, 0);

    uart_set_mode(uart_num, UART_MODE_RS485_HALF_DUPLEX);
}
void get_uart_data(char* buffer, size_t buffer_size)
{
    uint8_t* data = (uint8_t*)malloc(BUF_SIZE);

    const char* test_str = "3101valuer\n\r";

    for (int ctr = 0; ctr < 5; ctr++) {
        uart_flush(UART_PORT);
        uart_write_bytes(UART_PORT, test_str, strlen(test_str));
        vTaskDelay(pdMS_TO_TICKS(100));
        int len = uart_read_bytes(UART_PORT, data, BUF_SIZE, PACKET_READ_TICS);
        snprintf(buffer, buffer_size, "%.*s", len, (char*)data);
        printf("raw received data: '%.*s'\n", len, (char*)data);
        // vTaskDelay(pdMS_TO_TICKS(500));
        break;
    }

    free(data);
}
