#ifndef UART_HANDLER_H
#define UART_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_system.h"

void uart_init(void);
int sendData(const char* data);
static void tx_task();
void rx_task(char rxdata[]);

#ifdef __cplusplus
}
#endif

#endif /* UART_HANDLER_H */
