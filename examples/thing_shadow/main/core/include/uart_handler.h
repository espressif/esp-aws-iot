#ifndef UART_HANDLER_H
#define UART_HANDLER_H

#include "driver/uart.h"

#define UART_NUM UART_NUM_0
#define UART_BAUD_RATE 38400
#define UART_DATA_BITS UART_DATA_8_BITS
#define UART_PARITY UART_PARITY_DISABLE
#define UART_STOP_BITS UART_STOP_BITS_1
#define UART_FLOW_CTRL UART_HW_FLOWCTRL_DISABLE
#define UART_RX_FLOW_CTRL_THRESH 122
#define UART_BUFFER_SIZE (1024 * 2)

void uart_initialize();
void get_uart_data(char* buffer, size_t buffer_size);


#endif  // UART_HANDLER_H
