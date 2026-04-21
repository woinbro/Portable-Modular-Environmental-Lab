#include "battery.h"
#include "display.h"
#include "buttons.h"
#include "driver/rtc_io.h"

#define BTN_SEL 21  

void goToLightSleep() {
    drawLinuxStyle(true); 
    delay(500);

    
    while(digitalRead(BTN_SEL) == LOW) { delay(10); }
    
    gpio_wakeup_enable((gpio_num_t)BTN_SEL, GPIO_INTR_LOW_LEVEL);
    esp_sleep_enable_gpio_wakeup();
    tft.enableDisplay(false);
    Serial.println("Легкая гибернация...");
    esp_light_sleep_start();


    
    tft.initR(INITR_MINI160x80);
    tft.setRotation(3);
    tft.enableDisplay(true);
    
    drawLinuxStyle(false); 
    delay(800);

    btn2 = false; 
    tft.fillScreen(ST7735_BLACK); 
}

void goToDeepSleep() {
    drawLinuxStyle(true);
    delay(500);

    while(digitalRead(BTN_SEL) == LOW) { delay(10); }
    
    esp_deep_sleep_enable_gpio_wakeup(1 << (gpio_num_t)BTN_SEL, ESP_GPIO_WAKEUP_GPIO_LOW);

    Serial.println("Глубокая гибернация...");

    tft.enableDisplay(false);
    esp_deep_sleep_start();
}