#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>

#define BTN_LEFT 10
#define BTN_RIGHT 20
#define BTN_SEL 21  

extern boolean btn1;
extern boolean btn2;
extern boolean btn3;

void setupButtons();
void checkButtons();

#endif