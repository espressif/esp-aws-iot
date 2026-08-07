/**
 * @file nvs_platform.c
 * @author Hamxa Islam {hamzaislam170@gamil.com}
 * @brief Remotewell_PRO
 * @version 0.1
 * @date 2022-11-04
 *
 * EPTeck Technologies Gmbh    2022
 *
 */

//#ifdef CONFIG_HTTP_ENABLE &&CONFIG_WIFI_ENABLE

#include "nvs_platform.h"

#include <string.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "sdkconfig.h"
#include "esp_log.h"

static char *TAG="NVS []";
/**
 * @brief
 *
 * @param param
 */
bool nvs_slot_exit(esp_err_t err)
{
    switch (err)
    {
    case ESP_OK:
        return true;
    case ESP_ERR_NVS_NOT_FOUND:
        return false;
    default:
        // ESP_LOGI("NVS ERROR :","***** SEGMENTATION FAULT *****");
        break;
    }
    return false;
}
/**
 * @brief
 *
 * @param param
 */

void nvs_init(void *param)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
}

/**
 * @brief
 *
 * @param param
 */

void nvs_perform_number(char *key, uint32_t *value, e_nvs_param_t type, bool read_write)
{
    esp_err_t err;
    nvs_handle_t my_handle;

    err = nvs_open("storage", NVS_READWRITE, &my_handle);

    if (err != ESP_OK)
    {
        //  ESP_LOGE(TAG,"NVS ERROR:", "NOT ABLE TO OPEN NVS");
       
    }
    else
    {
        /*< CXX COMPILER >*/
    }
    if (read_write)
    {
       uint32_t valwr=*(value);
        switch (type)
        {
        case U8:
            err = nvs_set_u8(my_handle, (const char *)key, (uint8_t)valwr);
            break;
        case U16:
            err = nvs_set_u16(my_handle, (const char *)key, (uint16_t)valwr);
            break;
        case U32:
            err = nvs_set_u32(my_handle, (const char *)key, (uint32_t)valwr);
            break;
        case STR:
            err = nvs_set_str(my_handle, (const char *)key, (const char *)value);
            break;

        default:
            break;
        }
    }
    /*< Read from NVS >*/
    else
    {
        size_t required_size;
       uint32_t valwr=*(value);
        switch (type)
        {
        case U8:
            err = nvs_get_u8(my_handle, (const char *)key, (uint8_t *)value);
            nvs_slot_exit(err) ? true : nvs_set_u8(my_handle, (const char *)key, (uint8_t)valwr);
            break;
        case U16:
            err = nvs_get_u16(my_handle, (const char *)key, (uint16_t *)value);
            nvs_slot_exit(err) ? true : nvs_set_u16(my_handle, (const char *)key, (uint16_t)valwr);
            break;
        case U32:
            err = nvs_get_u32(my_handle, (const char *)key, (uint32_t *)value);
            nvs_slot_exit(err) ? true : nvs_set_u32(my_handle, (const char *)key, (uint32_t)valwr);
            break;
        case STR:
            err = nvs_get_str(my_handle, (const char *)key, NULL, &required_size);
            nvs_slot_exit(err) ? nvs_get_str(my_handle, (const char *)key, (const char *)value, &required_size)
                               : nvs_set_str(my_handle, (const char *)key, (const char *)value);
            break;

        default:
            break;
        }
    }
    nvs_commit(my_handle);
    nvs_close(my_handle);
}
void nvs_perform_str(char *key, char *value, e_nvs_param_t type, bool read_write)
{
    esp_err_t err;
    nvs_handle_t my_handle;

    err = nvs_open("storage", NVS_READWRITE, &my_handle);

    // if (err != ESP_OK)
    // {
    //     // ESP_LOGI("NVS ERROR:", "NOT ABLE TO OPEN NVS");
    // }
    // else
    // {
    //     /*< CXX COMPILER >*/
    // }

    if (read_write)
    {

        err = nvs_set_str(my_handle, (const char *)key, (const char *)value);
    }

    /*< Read from NVS >*/
    else
    {
        size_t required_size;

        err = nvs_get_str(my_handle, (const char *)key, NULL, &required_size);
        nvs_slot_exit(err) ? nvs_get_str(my_handle, (const char *)key, (const char *)value, &required_size)
                           : nvs_set_str(my_handle, (const char *)key, (const char *)value);
    }
    nvs_commit(my_handle);
    nvs_close(my_handle);
}
