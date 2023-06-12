#ifndef UART_HANDLER_H_
#define UART_HANDLER_H_

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Initializes the UART module.
 *
 * @return true if the initialization is successful, false otherwise.
 */
bool uart_initialize();

/**
 * @brief Retrieves data from the UART module.
 *
 * @param buffer The buffer to store the retrieved data.
 * @param buffer_size The size of the buffer.
 * @return true if data retrieval is successful, false otherwise.
 */
bool get_uart_data(char* buffer, size_t buffer_size);

#endif /* UART_HANDLER_H_ */
