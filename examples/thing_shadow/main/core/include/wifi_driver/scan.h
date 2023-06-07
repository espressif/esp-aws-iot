#ifndef _SCAN_H_
#define _SCAN_H_

/**
 * @brief WiFiScan() this function disconnects the ESP to Scan for available wifi interfaces available. It calls another
 * function CreateScanJsonPacket() to save the found wifis in a json packet. it is used in the web server scan api
 * @param None
 * @return None
 */
void WiFiScan(char *pscan_list);

#endif //_SCAN_H_