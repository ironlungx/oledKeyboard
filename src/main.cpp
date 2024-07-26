#include <Arduino.h>
#include <U8g2lib.h>

#include "keyboard.h"

#define keyH 27
#define keyJ 26
#define keyK 25
#define keyL 33

#define keySEL 23

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

int checkInput() {
  while (true) {
    if (digitalRead(keyH) == 1) {
      return ACTION_RIGHT;
    } // Right
    if (digitalRead(keyJ) == 1) {
      return ACTION_DOWN;
    } // Down
    if (digitalRead(keyK) == 1) {
      return ACTION_UP;
    } // Up
    if (digitalRead(keyL) == 1) {
      return ACTION_LEFT;
    } // Left

    if (digitalRead(keySEL) == 1) {
      return ACTION_SEL;
    }
  }
}

void setup() {
  u8g2.begin();

  Serial.begin(115200);

  pinMode(keyH, INPUT_PULLDOWN);
  pinMode(keyJ, INPUT_PULLDOWN);
  pinMode(keyK, INPUT_PULLDOWN);
  pinMode(keyL, INPUT_PULLDOWN);

  pinMode(keySEL, INPUT_PULLDOWN);

  String x = keyboard::keyboard(u8g2, checkInput);

  String msg = "You typed " + x;

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2.drawStr(2, 12, msg.c_str());
  u8g2.sendBuffer();
}

void loop() {}
