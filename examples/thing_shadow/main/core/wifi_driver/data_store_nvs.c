#include <string.h>
#include <stdio.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <esp_log.h>
#include "wifi_driver/data_store_nvs.h"

static const char *TAG = "Data Store NVS";

char ssid[30] = "", pswd[30] = "", tank[30] = "";
char user_name[30] = {0}, user_pswd[30] = {0}, user_device_num[5] = {0};

/**
 * @brief ReadStringNVS() is local function that reads the specified string key and returns the string value of the key.
 *
 * @param char key
 * @return char*
 */
int ReadStringNVS(char *key, char *value)
{

    esp_err_t err = nvs_flash_init();
    nvs_handle_t my_handle;

    err = nvs_open("storage", NVS_READWRITE, &my_handle);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {
        ESP_LOGD(TAG, "Done\n");
        ESP_LOGD(TAG, "Reading SSID from NVS ... ");

        size_t len;

        nvs_get_str(my_handle, key, NULL, &len);
        if (len == 0)
        {
            return 0;
        }

        err = nvs_get_str(my_handle, key, value, &len);

        switch (err)
        {
        case ESP_OK:
            ESP_LOGD(TAG, "Done\n");
            return 1;
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            ESP_LOGD(TAG, "The value is not initialized yet!\n");
            break;
        default:
            ESP_LOGD(TAG, "Error (%s) reading!\n", esp_err_to_name(err));
        }
        nvs_close(my_handle);
    }
    return 2;
}

void WriteStrNVS(char *key, char *value)
{
    esp_err_t err = nvs_flash_init();

    ESP_LOGD(TAG, "\n");
    ESP_LOGD(TAG, "Opening Non-Volatile Storage (NVS) handle... ");

    nvs_handle_t my_handle;

    err = nvs_open("storage", NVS_READWRITE, &my_handle);

    if (err != ESP_OK)
    {
        ESP_LOGD(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {
        ESP_LOGD(TAG, "Done\n");
        ESP_LOGD(TAG, "Storing key (%s) and value (%s) in NVS ... ", key, value);
        // Write
        err = nvs_set_str(my_handle, key, value);
        // Commit written value.
        nvs_close(my_handle);
    }
    ESP_LOGD(TAG, "\n");
}
void GetWifiCred()
{
    int err;

    char ssid_buff[30];
    char pswd_buff[30];
    char tank_buff[30];

    err = ReadStringNVS("ssid", ssid_buff);
    if (!err)
    {
        WriteStrNVS("ssid", "dummy");
    }
    err = ReadStringNVS("pswd", pswd_buff);
    if (!err)
    {
        WriteStrNVS("pswd", "dummy");
    }


    if (ssid_buff != NULL && pswd_buff != NULL)
    {
        sprintf(ssid, "%s", ssid_buff);
        sprintf(pswd, "%s", pswd_buff);
        sprintf(tank, "%s", tank_buff);

        ESP_LOGI("data_nvs", "ssid %s", ssid);
        ESP_LOGI("data_nvs", "pswd %s", pswd);
    }
    else
    {
        sprintf(ssid,"%s","");
        sprintf(pswd,"%s","");

    }
}

void NVSInit()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}