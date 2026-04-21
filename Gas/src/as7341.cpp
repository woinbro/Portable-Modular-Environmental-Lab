#include "as7341.h"
#include "arrays.h"
#include "display.h"

#define I2C_SDA 8
#define I2C_SCL 9

Adafruit_AS7341 as7341;

void asInit(){
    Serial.println("Инициализация AS7341...");
    Wire.begin(I2C_SDA, I2C_SCL);

    if (!as7341.begin()) {
        Serial.println("AS7341 не найден!");
        while (1);
    }
    Serial.println("AS7341 найден!");

    as7341.setATIME(100);      // Время интеграции
    as7341.setASTEP(999);      // Шаг
    as7341.setGain(AS7341_GAIN_256X);
    Serial.println("Настройка завершена");
}

void readSpectr(){
    as7341.enableLED(true);
    uint16_t readings[12];
    if (as7341.readAllChannels(readings)) {
        for(int i=0; i<12; i++) spectr[i] = readings[i]; // Записываем в наш список данные
    }
    as7341.enableLED(false);
}