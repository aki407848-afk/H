#ifndef CONFIG_H
#define CONFIG_H

// Настройки WiFi
#define MAX_NETWORKS 20
#define DEAUTH_REASON 1 // 1 = Unspecified reason
#define PACKETS_PER_SECOND 50 // Скорость отправки пакетов

// Настройки Serial
#define SERIAL_BAUD 115200

// Пины (для C3 Super Mini обычно нет встроенного LED для индикации, можно оставить пустым)
#define LED_PIN -1 

#endif
