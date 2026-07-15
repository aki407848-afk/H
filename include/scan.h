#ifndef SCAN_H
#define SCAN_H

#include "config.h" // <--- Сначала конфиг, чтобы знать MAX_NETWORKS
#include <Arduino.h>
#include <WiFi.h>

struct WifiNetwork {
    String ssid;
    uint8_t bssid[6];
    int channel;
    int rssi;
};

extern WifiNetwork networks[MAX_NETWORKS];
extern int networkCount;

void scanNetworks();
void printNetworkList();

#endif
