#include <Arduino.h>
#include "display.h"
#include "bme680.h"
#include "as7341.h"
#include "mics-6814.h"
#include "buttons.h"
#include "battery.h"
#include <esp_bt.h>
#include <esp_wifi.h>


unsigned long lastUpdate = 0;

void setup() {
  Serial.begin(9600);
  // Для экономии батареи
  esp_wifi_stop();
  esp_wifi_deinit();
  esp_bt_controller_disable();


  setupButtons(); // Инициализация кнопок
  gpio_hold_en((gpio_num_t)BTN_SEL);
  displayInit(); // Инициализация дисплея
  drawLinuxStyle(false);

  setupGas(); // Инициализация MICS-6814
  bmeInit(); // Инициализация BME-680
  bmeCalibrate(); // Калибровка BME-680
  asInit(); // Инициализация AS-7341
  clearScreen(); // Очистка экрана
}

void loop() {
  checkButtons();
  mainMenu();
}