#ifndef BME_MODULE_H
#define BME_MODULE_H
#include <Adafruit_BME680.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "arrays.h"


void bmeInit();
void bmeRead();
void bmeCalibrate();
int calculateIAQ(float humidity, float gas_resistance);

#endif