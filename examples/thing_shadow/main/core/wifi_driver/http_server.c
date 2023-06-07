#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <sys/param.h>
#include <esp_http_server.h>

#include "esp_netif.h"

#include "wifi_driver/data_store_nvs.h"
#include "wifi_driver/http_server.h"
#include "wifi_driver/wifi_manager.h"
#include "cJSON.h"
#include "wifi_driver/scan.h"

static const char *TAG = "HTTP server";
char scan_list[500] = {0};

/* An HTTP GET handler */
static esp_err_t wifi_scan_get_handler(httpd_req_t *req)
{
    WiFiScan(scan_list);

    const char *resp_str = (const char *)scan_list;

    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static const httpd_uri_t wifiscan = {
    .uri = "/api/wifiscan",
    .method = HTTP_GET,
    .handler = wifi_scan_get_handler,
    .user_ctx = NULL};

/* An HTTP POST handler */
static esp_err_t wifi_cred_post_handler(httpd_req_t *req)
{
    char buf[100];
    int ret, remaining = req->content_len;

    while (remaining > 0)
    {
        /* Read the data for the request */
        if ((ret = httpd_req_recv(req, buf,
            MIN(remaining, sizeof(buf)))) <= 0)
        {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT)
            {
                /* Retry receiving if timeout occurred */
                continue;
            }
            return ESP_FAIL;
        }

        remaining -= ret;

        /* Log data received */
        ESP_LOGD(TAG, "=========== RECEIVED DATA ==========");
        ESP_LOGD(TAG, "%.*s", ret, buf);
        ESP_LOGD(TAG, "====================================");

        cJSON *recv_json = cJSON_Parse(buf);
        cJSON *ssid_buff = cJSON_GetObjectItemCaseSensitive(recv_json, "ssid");
        cJSON *pswd_buff = cJSON_GetObjectItemCaseSensitive(recv_json, "pswd");
        // cJSON *tank_buff = cJSON_GetObjectItemCaseSensitive(recv_json, "tank");

        if (cJSON_IsString(ssid_buff) && (ssid_buff->valuestring != NULL) && (cJSON_IsString(pswd_buff) && (pswd_buff->valuestring != NULL)))
        {
            ESP_LOGD(TAG, "Checking ssid \"%s\"\n", ssid_buff->valuestring);
            ESP_LOGD(TAG, "Checking password \"%s\"\n", pswd_buff->valuestring);
            // ESP_LOGD(TAG, "Checking tank name \"%s\"\n", tank_buff->valuestring);

            WriteStrNVS("ssid", ssid_buff->valuestring);
            WriteStrNVS("pswd", pswd_buff->valuestring);
            // WriteStrNVS("tank", tank_buff->valuestring);

            GetWifiCred(); // load new credentials
            Wifi_STA();
        }
        else
        {
            /* Send back the same data */
            httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Wrong key value pairs");
        }
        cJSON_Delete(recv_json);
        esp_restart();
    }

    // End response
    httpd_resp_send_chunk(req, NULL, 0);

    return ESP_OK;
}

static const httpd_uri_t wificredential = {
    .uri = "/api/wificredential",
    .method = HTTP_POST,
    .handler = wifi_cred_post_handler,
    .user_ctx = NULL};

esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    /* For any other URI send 404 and close socket */
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "404 Error Not Found");
    return ESP_FAIL;
}

httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGD(TAG, "Starting server on port: '%d'", config.server_port);

    if (httpd_start(&server, &config) == ESP_OK)
    {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");

        httpd_register_uri_handler(server, &wifiscan);
        httpd_register_uri_handler(server, &wificredential);
        //httpd_register_uri_handler(server, &parsecredential);
        return server;
    }

    ESP_LOGD(TAG, "Error starting server!");
    return NULL;
}

static void stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server
    httpd_stop(server);
}

void disconnect_handler(void *arg, esp_event_base_t event_base,
                        int32_t event_id, void *event_data)
{
    httpd_handle_t *server = (httpd_handle_t *)arg;

    if (*server)
    {
        ESP_LOGD(TAG, "Stopping webserver");

        stop_webserver(*server);

        *server = NULL;
    }
}

void connect_handler(void *arg, esp_event_base_t event_base,
                     int32_t event_id, void *event_data)
{
    httpd_handle_t *server = (httpd_handle_t *)arg;

    if (*server == NULL)
    {
        ESP_LOGD(TAG, "Starting webserver");

        *server = start_webserver();
    }
}
void WebServerEventRegister()
{
    static httpd_handle_t server = NULL;

    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_AP_STAIPASSIGNED, &connect_handler, &server));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, &disconnect_handler, &server));
}
