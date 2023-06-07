#include "esp_sntp.h"
#include "esp_tls.h"
#include <time.h>
#include "ping_time.h"
#include "esp_log.h"

#define TAG "Time:"

bool sntp_resta = false;
long ttime;

/**
 * @brief This function is responsible for converting time string to Unix 
 * 
 * @param ptr char*
 * @return long 
 */
static long ConvertParseTimeToUnix(char *ptr)
{
    struct tm result;
    memset(&result, 0, sizeof(struct tm));
    time_t epoch;
    strptime(ptr, "%Y-%m-%d %R", &result);
    epoch = mktime(&result);
    return (epoch);
}
/**
 * @brief Notification for time sync
 * 
 * @param tv 
 */
static void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "Notification of a time synchronization event");
}
/**
 * @brief Intialization SNTP server
 * 
 */
static void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "time.google.com");
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    sntp_init();
}
/**
 * @brief Getting time from SNTP server
 * 
 */
static void obtain_time(void)
{
    if (!sntp_restart())
    {
        initialize_sntp();
    }
    else
    {
        //do nothing
    }
    time_t now = 0;
    int retry = 0;
    const int retry_count = 25;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count)
    {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        if (retry_count == 24)
        {
            sntp_resta = true;
        }
    }
    time(&now);
}
long GetStandardTime()
{
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    // Is time set? If not, tm_year will be (1970 - 1900).
    if (timeinfo.tm_year < (2016 - 1900) || sntp_resta)
    {
        ESP_LOGI(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
        obtain_time();
        // update 'now' variable with current time
        time(&now);
        sntp_resta = false;
    }
    char strftime_buf[64];

    setenv("TZ", "GMT-2", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "Time : %s", strftime_buf);

    if (sntp_get_sync_mode() == SNTP_SYNC_MODE_SMOOTH)
    {
        struct timeval outdelta;
        while (sntp_get_sync_status() == SNTP_SYNC_STATUS_IN_PROGRESS)
        {
            adjtime(NULL, &outdelta);
            ESP_LOGI(TAG, "Waiting for adjusting time ... outdelta = %li sec: %li ms: %li us",
            outdelta.tv_sec,
            outdelta.tv_usec / 1000,
            outdelta.tv_usec % 1000);

            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
    }
    return (now);
}