#ifndef DEAUTH_H
#define DEAUTH_H

#include "config.h" // <--- Сначала конфиг
#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>

// Структура Deauth пакета IEEE 802.11
typedef struct {
    uint8_t frame_control[2];
    uint8_t duration[2];
    uint8_t dest[6];
    uint8_t src[6];
    uint8_t bssid[6];
    uint8_t seq[2];
    uint16_t reason;
} __attribute__((packed)) deauth_frame_t;

extern bool isAttacking;
extern int targetIndex;

void startDeauth(int index);
void stopDeauth();
void sendDeauthPackets();

#endif
