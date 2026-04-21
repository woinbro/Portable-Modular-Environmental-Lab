#include "bme680.h"
#include <Wire.h>

#define BME_SDA 8    
#define BME_SCL 9     

#define BME_ADDR 0x77 
Adafruit_BME680 bme;

float t = 0, h = 0, p = 0, v = 0, g = 0;

void bmeInit() {
  Wire.begin(BME_SDA, BME_SCL);
  bme.begin(BME_ADDR, &Wire);
  Serial.println("Настройка завершена!");
}

void bmeCalibrate(){
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);
}

void bmeRead() {
  if (bme.performReading()) {
    bme680[0] = bme.temperature;          
    bme680[1] = bme.humidity;              
    bme680[2] = bme.pressure / 100.0;       
    bme680[3] = bme.readAltitude(1013.25);  
    bme680[4] = bme.gas_resistance;
  }
}

int calculateIAQ(float humidity, float gas_resistance) {
  
    float hum_weight = 0.25;
    float hum_score;
    float hum_reference = 40.0; // Идеальная влажность
    if (humidity >= hum_reference)
        hum_score = (100 - humidity) / (100 - hum_reference) * hum_weight;
    else
        hum_score = (humidity / hum_reference) * hum_weight;
    hum_score *= 100;


    float gas_weight = 0.75;
    float gas_score;
    float gas_reference = 250000; // Базовое сопротивление воздуха
    
    if (gas_resistance > gas_reference) gas_resistance = gas_reference;
    gas_score = (gas_resistance / gas_reference) * gas_weight * 100;

  
    float air_quality_score = hum_score + gas_score;
    int iaq = (100 - air_quality_score) * 5; 
    
    if (iaq < 0) iaq = 0;
    if (iaq > 500) iaq = 500;
    
    return iaq;
}