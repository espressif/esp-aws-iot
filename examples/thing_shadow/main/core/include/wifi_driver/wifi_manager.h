#ifndef _WIFI_MANAGER_H
#define _WIFI_MANAGER_H



extern uint8_t wifi_state;
extern int disconnectivity_count;
/**
 * @brief initialzing wifi and webserver
 *
 */
void WifiInit();
/**
 * @brief AP mode configurations and initialization
 *
 */
void Wifi_AP(void);
/**
 * @brief STA mode configurations and initialization
 *
 */
void Wifi_STA(void);
/**
 * @brief STA+AP mode configurations and initialization
 *
 */
void Wifi_AP_STA(void);



#endif
