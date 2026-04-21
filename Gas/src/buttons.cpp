#include "buttons.h"

boolean btn1 = false;
boolean btn2 = false;
boolean btn3 = false;

void setupButtons(){
    pinMode(BTN_LEFT, INPUT_PULLUP);
    pinMode(BTN_SEL, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
}

void checkButtons(){
    if (digitalRead(BTN_LEFT) == LOW) {
    btn1 = true;
  }
  
  if (digitalRead(BTN_SEL) == LOW) {
    btn2 = true;
  }
  
  if (digitalRead(BTN_RIGHT) == LOW) {
    btn3 = true;
  }
}