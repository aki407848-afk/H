#include "scan.h"
#include "config.h"
#include <stdio.h>

WifiNetwork networks[MAX_NETWORKS];
int networkCount = 0;

void scanNetworks() {
    Serial.println("Scanning...");
    WiFi.mode(WIFI_MODE_STA);
    WiFi.disconnect();
    delay(100);
    
    networkCount = WiFi.scanNetworks();
    
    if (networkCount == 0) {
        Serial.println("No networks found.");
        return;
    }
    
    // Ограничиваем количество сетей
    if (networkCount > MAX_NETWORKS) {
        networkCount = MAX_NETWORKS;
    }
    
    for (int i = 0; i < networkCount; i++) {
        networks[i].ssid = WiFi.SSID(i);
        networks[i].channel = WiFi.channel(i);
        networks[i].rssi = WiFi.RSSI(i);
        memcpy(networks[i].bssid, WiFi.BSSID(i), 6);
    }
    
    printNetworkList();
}

void printNetworkList() {
    Serial.println("-----------------------------------");
    Serial.println("ID | SSID             | CH | RSSI");
    Serial.println("-----------------------------------");
    
    for (int i = 0; i < networkCount; i++) {
        Serial.printf("%-2d | %-16s | %2d | %4d\n", 
            i, 
            networks[i].ssid.substring(0, 16).c_str(), 
            networks[i].channel, 
            networks[i].rssi);
    }
    Serial.println("-----------------------------------");
    Serial.println("Enter ID to attack, 'scan' to rescan, 'stop' to stop.");
}
