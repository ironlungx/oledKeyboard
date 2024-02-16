#include <Arduino.h>
#include <U8g2lib.h>
#include "bitmaps.h"

#define VRx 32
#define VRy 25

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

const char *letters[][10] = {
    {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p"},
    {"a", "s", "d", "f", "g", "h", "j", "k", "l", " "},
    {"<caps>", "z", "x", "c", "v", "b", "n", "m", "<caps>", "<rm>"},
    {" ", " ", " ", " ", " ", " ", " ", " ", " ", " "}};

const char *capitalLetters[][10] = {
    {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"},
    {"A", "S", "D", "F", "G", "H", "J", "K", "L", " "},
    {"<caps>", "Z", "X", "C", "V", "B", "N", "M", "<caps>", "<rm>"},
    {" ", " ", " ", " ", " ", " ", " ", " ", " ", " "}};

bool caps = false;
void render_menu(int pX, int pY, String text) {

  u8g2.clearBuffer();

  // 0, 9    ->    128, 9
  // 0, 23   ->    128, 9
  // 0, 37   ->    128, 37
  // 0, 51   ->    128, 51

  for (int i = 9; i <= 51; i += 14) {
    u8g2.drawLine(0, i, 128, i);
  }

  //  9, 15  -> 9, 55
  //  9, 29
  for (int i = 15; i <= 120; i += 12) {
    u8g2.drawLine(i, 9, i, 64);
  }

  // u8g2_font_6x13_tr

  // for (int i = 0; i <= 9;  /i++)
  // {
  // u8g2.setFont(u8g2_font_6x13_tr);
  // u8g2.drawStr(14*i+i, 12, (const char*)letters[0][i]);
  // u8g2.drawPixel(14*i, 15);
  // }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 10; j++) {
      // Calculate coordinates for each character
      int x = j * 12 +
              6; // 12 is the grid cell width, 6 is half of it for centering
      int y = i * 14 + 18 +
              1; // 14 is the grid cell height, 18 is half of it for centering

      u8g2.setFont(u8g2_font_6x13_tr);
      
      if (strcmp(letters[i][j], "<caps>") == 0) {
        if (caps || (j == pX && i == pY))
        {
          u8g2.setFont(u8g2_font_6x12_t_symbols);
          u8g2.drawButtonUTF8(x, y, U8G2_BTN_INV, 0, 1, 1, 
                              "⇑");
        }
        else {
          u8g2.setFont(u8g2_font_6x12_t_symbols);
          u8g2.drawUTF8(x, y, "⇑");
        }

        continue;
      }

      if (strcmp(letters[i][j], "<rm>") == 0) {
        if (j == pX && i == pY) {
          u8g2.setFont(u8g2_font_6x12_t_symbols);
          u8g2.drawButtonUTF8(x, y, U8G2_BTN_INV, 0, 1, 1, "←");
        }
        else {
          u8g2.setFont(u8g2_font_6x12_t_symbols);
          u8g2.drawUTF8(x, y, "←");
        }
        continue; 
      }

      // ⑫ for numbers
      // # for symbols
      if (caps) { 
        if (j == pX && i == pY)
          u8g2.drawButtonUTF8(x, y, U8G2_BTN_INV, 0, 1, 1,
                              (const char *)capitalLetters[i][j]);
        else
          u8g2.drawStr(x, y, (const char *)capitalLetters[i][j]);
      } else {
        if (j == pX && i == pY)
          u8g2.drawButtonUTF8(x, y, U8G2_BTN_INV, 0, 1, 1,
                              (const char *)letters[i][j]);
        else
          u8g2.drawStr(x, y, (const char *)letters[i][j]);
      }
    }
  }
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(2, 7, text.c_str());

  int cursorX = 2 + u8g2.getStrWidth(text.c_str()) + 2;
  u8g2.drawVLine(cursorX, 0, 9);

  u8g2.sendBuffer();
}

void setup(void) {
  u8g2.begin();

  Serial.begin(115200);
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(15, INPUT_PULLUP);
}

int pointerX = 0;
int pointerY = 0;
String text = "";


void loop(void) {
  // Handle joystick input to move the pointer
  // Assume joystick inputs change pointerX and pointerY values

  // Example code to simulate joystick input
  if (analogRead(VRx) > 2500 && analogRead(VRy) < 100) {
    pointerX = max(0, pointerX - 1); // Ensure poin /terX stays within bounds
  } else if (analogRead(VRx) > 2500 && analogRead(VRy) > 3000) {
    pointerX = min(9, pointerX + 1); // Ensure pointerX st /ays within bounds
  }

  if (analogRead(VRx) > 3000 && analogRead(VRy) > 2500) {
    pointerY = max(0, pointerY - 1); // Ensure pointerY stays within bounds
  } else if (analogRead(VRx) < 100 && analogRead(VRy) > 2500) {
    pointerY = min(3, pointerY + 1); // Ensure pointerY stays within bounds
  }

  if (digitalRead(15) == 0) {
    if (strcmp(letters[pointerY][pointerX], "<caps>") == 0) {
      Serial.printf("caps\n");
      caps = (!caps);
      goto skip_append;
    }
    if (strcmp(letters[pointerY][pointerX], "<rm>") == 0) {
      Serial.printf("rm\n");
      text.remove(text.length() - 1);
      goto skip_append;
    }

    if (caps) {
      String x = String(letters[pointerY][pointerX]);
      x.toUpperCase();
      text += x;
      
      goto skip_append;
    }

    text += letters[pointerY][pointerX];
    Serial.printf("%s\n", text.c_str());
  }
skip_append:

  // Render the menu with the updated pointer position
  render_menu(pointerX, pointerY, text);


  delay(100); // Add a small delay to avoid rapid menu updates

  // Serial.printf("X: %d\tY: %d\t\tSW: %d\n", analog sead(VRx), analogRead(VRy), digitalRead(15));
}
