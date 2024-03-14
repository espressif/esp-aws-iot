/**
 * @file nvs_platform.h
 * @author Hamxa Islam {hamzaislam170@gamil.com}
 * @brief Remotewell_PRO
 * @version 0.1
 * @date 2022-11-04
 *
 * EPTeck Technologies Gmbh
 *
 */

#ifndef _NVS_PLATFORM_
#define _NVS_PLATFORM_

#include "stdlib.h"
#include "stdint.h"
#include <stdio.h>
#include <string.h>
#include "stdbool.h"

#include "sdkconfig.h"

/*:) NVS Enum and Struct-------------------------------------------------------*/

typedef enum
{
    U8,
    U16,
    U32,
    STR
} e_nvs_param_t;

/*:) NVS External Function -------------------------------------------------------*/

void nvs_perform_number(char *key, uint32_t *value, e_nvs_param_t type, bool read_write);
void nvs_perform_str(char *key, char *value, e_nvs_param_t type, bool read_write);
void nvs_init(void *param);
#endif