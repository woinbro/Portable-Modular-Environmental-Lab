#ifndef DISPLAY_MODULE_H
#define DISPLAY_MODULE_H

#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

#define BLACK 0x0000
#define WHITE 0xFFFF
#define GRAY  0x8410

#define TFT_SDA  6  
#define TFT_SCLK  4  
#define TFT_CS    7  
#define TFT_DC    3 
#define TFT_RST   5

extern int currentScreen;
extern Adafruit_ST7735 tft;

void displayInit();
void mainMenu();
void drawAir();
void drawGas();
void drawLight();
void drawSystem();
void clearScreen();
void drawSleep();
void drawLinuxStyle(bool isShutdown);
void drawHeader(const char* path);
void drawIaq();
void drawSpectralGraph();
void drawLightAnalysis();

#endif