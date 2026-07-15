#include "scan.h"
#include "config.h"
#include <esp_wifi.h>
#include <esp_task_wdt.h>
#include <stdio.h>

WifiNetwork networks[MAX_NETWORKS];
int networkCount = 0;

void scanNetworks() {
    Serial.println("Scanning...");
    
    // Сбрасываем watchdog перед долгой операцией
    esp_task_wdt_reset();
    
    WiFi.mode(WIFI_MODE_STA);
    WiFi.disconnect();
    delay(100); // Даем время на переключение режима
    
    esp_task_wdt_reset(); // Еще раз кормим пса
    
    // Сканирование может занять до 5-10 секунд
    networkCount = WiFi.scanNetworks();
    
    esp_task_wdt_reset(); // И после сканирования
    
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
