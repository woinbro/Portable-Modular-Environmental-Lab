#include <mics6814.h>
#include "mics-6814.h"
#include "arrays.h"

#define PIN_CO  1
#define PIN_NO2 2
#define PIN_NH3 0

MICS6814 mics(PIN_CO, PIN_NO2, PIN_NH3);

void setupGas() {
  Serial.begin(9600);
  Serial.print("Calibrating Sensor");
  mics.calibrate();
  Serial.println("OK!");
}

void readGas() {
  gas[0] = mics.measure(CO);
  gas[1] = mics.measure(NO2);
  gas[2] = mics.measure(NH3);
}