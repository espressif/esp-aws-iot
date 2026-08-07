#ifndef _DATA_STORE_NVS_H
#define _DATA_STORE_NVS_H

/**
 * @brief WriteStrNVS() function stores the specified strings into the NVS. It requires a Key and a value
 *
 * @param char *key
 * @param char *value
 * @return None
 */
void WriteStrNVS(char *, char *);

/**
 * @brief A helper function to load the new wifi credentials i.e. SSID and Password from the NVS in single go. Stores the data in global variable ssid[30] and pswd[30]
 * @param None
 * @return None
 */
void GetWifiCred();
/**
 * @brief A helper function to load the new parse credentials i.e. userID, Password, and deviceNum from the NVS in single go.
 * Stores the data in global variable user_name[30],user_pswd[30], and user_device_num[5]
 * @param None
 * @return None
 */

void NVSInit();

extern char ssid[30], pswd[30],tank[30], user_name[30], user_pswd[30], user_device_num[5];

#endif // _DATA_STORE_NVS_H