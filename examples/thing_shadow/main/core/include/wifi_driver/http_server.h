#ifndef _HTTP_SERVER_H
#define _HTTP_SERVER_H

#include <esp_http_server.h>
#include <esp_event_base.h>

/**
 * @brief This function registers the URIs that will be accessible by the clients
 * @param None
 * @return httpd_handle_t
 */
httpd_handle_t start_webserver(void);

/**
 * @brief This function is responsible for stopping the web server
 *
 * @param arg
 * @param event_base
 * @param event_id
 * @param event_data
 */
void disconnect_handler(void *arg, esp_event_base_t event_base,
                        int32_t event_id, void *event_data);
/**
 * @brief This function is responsible for start the web server
 *
 * @param arg
 * @param event_base
 * @param event_id
 * @param event_data
 */
void connect_handler(void *arg, esp_event_base_t event_base,
                     int32_t event_id, void *event_data);
/**
 * @brief Register Connect and disconnect event handlers
 */
void WebServerEventRegister();

#endif // _HTTP_SERVER_H
