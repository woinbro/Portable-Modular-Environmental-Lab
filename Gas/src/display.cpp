#include "display.h"
#include "arrays.h"
#include "buttons.h"
#include "battery.h"
#include "bme680.h"
#include "mics-6814.h"
#include "as7341.h"
#include <Wire.h>

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_SDA, TFT_SCLK, TFT_RST);

int currentScreen = 0;
const int maxScreens = 7;

void clearScreen(){
    tft.fillScreen(ST7735_BLACK);
}

void displayInit() {
  tft.initR(INITR_MINI160x80);
  tft.fillScreen(ST7735_BLACK);
  Serial.println(F("Дисплей инициализирован"));
  tft.setRotation(3);
}

void drawHeader(const char* path) {
    tft.setCursor(0, 2);
    tft.print("#root@gera_os:");
    tft.print(path);
}

void drawNavigation() {
    tft.setTextSize(1);
    tft.setTextColor(ST7735_WHITE, ST7735_BLACK);

    tft.setCursor(0, 10);
    tft.print("--------------------------");

    tft.setCursor(2, 35);
    tft.print("<");

    tft.setCursor(152, 35);
    tft.print(">");
}
void mainMenu() {
    // Навигация
    if (btn3) { btn3 = false; currentScreen = (currentScreen + 1) % 8; tft.fillScreen(ST7735_BLACK); }
    if (btn1) { btn1 = false; currentScreen = (currentScreen - 1 + 8) % 8; tft.fillScreen(ST7735_BLACK); }
    
    drawNavigation();
    tft.setTextColor(ST7735_WHITE, ST7735_BLACK);

    switch(currentScreen) {
        case 0: drawAir(); break;
        case 1: drawIaq(); break;
        case 2: drawGas(); break;
        case 3: drawLight(); break;
        case 4: drawLightAnalysis(); break;
        case 5: drawSpectralGraph(); break;
        case 6: 
            drawHeader("/sys/status");
            tft.setCursor(15, 25); tft.print("OS_VER: 1.1");
            tft.setCursor(15, 35); tft.print("CPU_FREQ: 160 MHz");
            tft.setCursor(15, 45); tft.print("UPTIME: "); tft.print(millis()/1000); tft.print(" s");
            break;
        case 7: 
            drawHeader("/sys/pwr");
            tft.setCursor(15, 30); tft.print("> Press SELECT to");
            tft.setCursor(15, 40); tft.print("  exec 'halt -p'");
            tft.setCursor(15, 50); tft.print("  light sleep");
            if (btn2) { btn2 = false; goToLightSleep(); }
            break;
        case 8: 
            drawHeader("/sys/pwr");
            tft.setCursor(15, 30); tft.print("> Press SELECT to");
            tft.setCursor(15, 40); tft.print("  exec 'halt -p'");
            tft.setCursor(15, 50); tft.print("  deep sleep");
            if (btn2) { btn2 = false; goToDeepSleep(); }
            break;
    }
}

void drawAir() {
    bmeRead();
    drawHeader("/sens/air");
    
    tft.setCursor(15, 25);
    tft.print("TEMPERATURE: "); 
    tft.print(bme680[0], 1); tft.print(" C  ");
    
    tft.setCursor(15, 35);
    tft.print("HUMIDITY: "); 
    tft.print(bme680[1], 1); tft.print(" %  ");
    
    tft.setCursor(15, 45);
    tft.print("PRESSURE: "); 
    tft.print(bme680[2], 0); tft.print(" hPa");
    
    tft.setCursor(15, 55);
    tft.print("HEIGHT: "); 
    tft.print(bme680[3], 0); tft.print(" meters");

    tft.setCursor(15, 65);
    tft.print("GAS: "); 
    tft.print(bme680[4], 0); tft.print(" kOhm");
}

void drawGas() {
    readGas();
    drawHeader("/sens/gas");
    
    tft.setCursor(15, 25);
    tft.print("CO_LEVEL: "); 
    tft.print(gas[0], 2); tft.print(" ppm ");
    
    tft.setCursor(15, 40);
    tft.print("NO2_LEVEL: "); 
    tft.print(gas[1], 3); tft.print(" ppm ");
    
    tft.setCursor(15, 55);
    tft.print("NH3_LEVEL: "); 
    tft.print(gas[2], 2); tft.print(" ppm ");
}

void drawIaq() {
    bmeRead();
    drawHeader("/sens/iaq");
    
    int currentIAQ = calculateIAQ(bme680[1], bme680[4]); // Влажность и Газ

    tft.setCursor(15, 25);
    tft.print("IAQ_INDEX:"); 
    tft.print(currentIAQ);

    tft.setCursor(15, 40);
    tft.print("STATUS ");
    if (currentIAQ <= 50){tft.print("[EXCELLENT]"); tft.fillRect(130, 40, 8, 8, ST7735_GREEN);}
    else if (currentIAQ <= 100){tft.print("[GOOD]"); tft.fillRect(130, 40, 8, 8, ST7735_ORANGE);}
    else if (currentIAQ <= 150){tft.print("[VENTILATE]"); tft.fillRect(130, 40, 8, 8, ST7735_CYAN);}
    else{tft.print("[CALAMITY]"); tft.fillRect(130, 40, 8, 8, ST7735_BLUE);}
}

void drawLight() { // Выводим Спектр по 11 каналам
    readSpectr();
    drawHeader("/sens/light");
    
    for (int i = 0; i < 6; i++) {
        
        tft.setCursor(15, 22 + (i * 9));
        tft.print("CH"); tft.print(i); tft.print(": "); tft.print(spectr[i]); tft.print("    ");
        
        
        tft.setCursor(85, 22 + (i * 9));
        tft.print("CH"); tft.print(i+5); tft.print(": "); tft.print(spectr[i+6]); tft.print("   ");

    }
}

void drawSpectralGraph() { // Гистограмма для спектра
    readSpectr();
    drawHeader("/sens/spectr");

    int barWidth = 12;  
    int spacing = 4;    
    int startX = 16;    
    int maxHeight = 35; 
    int baseY = 55;     

    float maxVal = 0;
    for (int i = 0; i < 8; i++) {
        if (spectr[i] > maxVal) maxVal = spectr[i];
    }
    if (maxVal < 1) maxVal = 1; 

    for (int i = 0; i < 8; i++) {
        int x = startX + i * (barWidth + spacing);
        int barHeight = (spectr[i] / maxVal) * maxHeight;

        
        tft.drawRect(x, baseY - maxHeight, barWidth, maxHeight, ST7735_WHITE);
        
        tft.fillRect(x + 1, baseY - maxHeight + 1, barWidth - 2, maxHeight - 2, ST7735_BLACK);

    
        for (int h = 0; h < barHeight; h += 3) {
            
            tft.drawFastHLine(x + 2, baseY - h - 1, barWidth - 4, ST7735_WHITE);
            
        }

        
        tft.setCursor(x + 2, baseY + 4);
        tft.setTextColor(ST7735_WHITE, ST7735_BLACK); // Текст на черном фоне, чтобы не мерцал
        tft.print("F"); tft.print(i + 1);
    }
}

void drawLightAnalysis() { // Оценка Спектра
    readSpectr();
    drawHeader("/sens/spectr");

    // Считываем каналы
    float blue = spectr[2] + spectr[3]; // F3(445nm) и F4(480nm)
    float red = spectr[6] + spectr[7];  // F7(630nm) и F8(680nm)
    float green = spectr[4] + spectr[5]; // F5(515nm) и F6(555nm)

    // 1. Оценка CCT
    float cct_ratio = blue / red;
    int cct = 3000 + (cct_ratio * 2000); // 3000K-7000K

    // 2. Оценка CRI 
    float cri_score = 100 - (abs(red - green) / 10.0); 

    tft.setCursor(15, 25);
    tft.print("CCT: "); tft.print(cct); tft.print(" K");
    
    tft.setCursor(15, 35);
    tft.print("COLOR_INDEX: "); tft.print(cri_score, 0); tft.print(" Ra");

    // 3. Биологическое влияние
    tft.setCursor(15, 45);
    tft.print("BLUE_ENERGY: "); tft.print(blue, 1);
    
    tft.setCursor(15, 55);
    if (blue > (red + green) * 0.5) {
        tft.print(" [!] CAUTION: BLUE HIGH ");
    } else {
        tft.setCursor(8, 55);
        tft.print(" [v] STATUS: BIO_SAFE  ");
    }
}

void drawLinuxStyle(bool isShutdown) { //Анимации (Загрузки, Отключения)
    tft.fillScreen(ST7735_BLACK);
    tft.setTextSize(1);
    tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
    tft.setCursor(0, 0);

    const char* bootMsgs[] = {
        "Start CPU[OK]",
        "Checking Memory[OK]",
        "Starting I2C[OK]",
        "Mounting Sensor[OK]",
        "Starting GeraOS[OK]"
        "Heating MICS6814..."
    };

    const char* shutMsgs[] = {
        "Stopping I2C Bus",
        "Saving sensor data",
        "Killing processes",
        "Power down."
    };

    for (int i = 0; i < 5; i++) {
        
        tft.print("[0."); 
        tft.print(random(100, 1000)); 
        tft.print("]");

        if (isShutdown) {
            tft.print(shutMsgs[i]);
            delay(200);
        } else {
            tft.print(bootMsgs[i]);
            delay(300);
        }
        tft.println();
    }

    if (isShutdown) {
        tft.println("\nSystem halted.");
    }
}