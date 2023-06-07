#include "uart_handler.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"

static const int RX_BUF_SIZE = 128;

#define TXD_PIN (GPIO_NUM_21)
#define RXD_PIN (GPIO_NUM_20)

#define UART_PORT UART_NUM_0

void uart_init(void) {
    const uart_config_t uart_config = {
        .baud_rate = 38400,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    // Configure UART parameters
    uart_param_config(UART_PORT, &uart_config);

    // Set UART pin numbers
    uart_set_pin(UART_PORT, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // Install UART driver with buffer sizes
    uart_driver_install(UART_PORT, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
}

int sendData(const char* data) {
    const int len = strlen(data);

    // Flush UART buffer and add delay for stability
    uart_flush(UART_PORT);
    uart_flush_input(UART_PORT);
    vTaskDelay(pdMS_TO_TICKS(500));

    // Write data to UART
    const int txBytes = uart_write_bytes(UART_PORT, data, len);
    return txBytes;
}

static void tx_task() {
    sendData("3101valuer\n\r");
    vTaskDelay(pdMS_TO_TICKS(500));
}

void rx_task(char rxdata[RX_BUF_SIZE]) {
    tx_task();
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    while (1) {
        const int rxBytes = uart_read_bytes(UART_PORT, data, RX_BUF_SIZE, pdMS_TO_TICKS(1000));
        ESP_LOGI("UART HANDLER", "Read %d bytes: '%s'", rxBytes, (char*) data);
        if (rxBytes > 0) {
            data[rxBytes] = '\0';
            ESP_LOGI("UART HANDLER", "Read %d bytes: '%s'", rxBytes, (char*) data);
            strcpy(rxdata, (char*) data); // Copy data to rxdata
            break;
        }
    }
    free(data);
}