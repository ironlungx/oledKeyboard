#include "esp32-hal-adc.h"
#include <Arduino.h>
#include <U8g2lib.h>

#define ACTION_LEFT 0
#define ACTION_RIGHT 1
#define ACTION_UP 2
#define ACTION_DOWN 3
#define ACTION_SEL 4

namespace keyboard {
const char *letters[][10] = {
    {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p"},
    {"a", "s", "d", "f", "g", "h", "j", "k", "l", " "},
    {"<caps>", "z", "x", "c", "v", "b", "n", "m", "<caps>", "<rm>"},
    {"<sym1>", " ", " ", " ", " ", " ", " ", " ", " ", "<ok>"}};

const char *capitalLetters[][10] = {
    {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"},
    {"A", "S", "D", "F", "G", "H", "J", "K", "L", " "},
    {"<caps>", "Z", "X", "C", "V", "B", "N", "M", "<caps>", "<rm>"},
    {"<sym1>", " ", " ", " ", " ", " ", " ", " ", " ", "<ok>"}};

const char *symbols1[][10] = {
    {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"},
    {"!", "@", "#", "$", "%", "^", "&", "*", "(", ")"},
    {"-", "=", "_", "+", "/", ":", ";", "?", "'", "<rm>"},
    {"<let>", "<sym2>", ".", ",", " ", " ", "[", "]", "{", "}"}};

const char *symbols2[][10] = {
    {"<", ">", "\\", "|", " ", " ", " ", " ", " ", " "},
    {" ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {" ", " ", " ", " ", " ", " ", " ", " ", " ", "<rm>"},
    {"<let>", "<sym1>", " ", " ", " ", " ", " ", " ", " ", " "}};
const char *(*currentLayer)[10] = letters;

bool caps = false;

void render_keyboard(int pX, int pY, String text, U8G2 u8g2) {

  u8g2.clearBuffer();

  for (int i = 9; i <= 51; i += 14) {
    u8g2.drawLine(0, i, 128, i);
  }
  for (int i = 15; i <= 120; i += 12) {
    u8g2.drawLine(i, 9, i, 64);
  }

  // u8g2_font_6x13_tr
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 10; j++) {
      // Calculate coordinates for each character
      int x = j * 12 +
              7; // 12 is the grid cell width, 6 is half of it for centering
      int y = i * 14 + 18 +
              2; // 14 is the grid cell height, 18 is half of it for centering

      u8g2.setFont(u8g2_font_6x13_tr);

      // Check if the current key to be rendered is the caps key
      if (strcmp(currentLayer[i][j], "<caps>") == 0) {
        // if the current selected key is the caps key OR caps is turned on
        // Draw the button with highlighting
        if (caps || (j == pX && i == pY)) {
          u8g2.setFont(u8g2_font_6x12_t_symbols);
          u8g2.drawButtonUTF8(x, y, U8G2_BTN_INV, 0, 1, 1, "⇑");
        }
        // else no higlight :)
        else {
          u8g2.setFont(u8g2_font_6x12_t_symbols);
          u8g2.drawUTF8(x, y, "⇑");
        }

        continue;
      }

      if (strcmp(currentLayer[i][j], "<rm>") == 0) {
        if (j == pX && i == pY) {
          u8g2.setFont(u8g2_font_6x12_t_symbols);
          u8g2.drawButtonUTF8(x, y, U8G2_BTN_INV, 0, 1, 1, "←");
        } else {
          u8g2.setFont(u8g2_font_6x12_t_symbols);
          u8g2.drawUTF8(x, y, "←");
        }
        continue;
      }

      if (strcmp(currentLayer[i][j], "<sym1>") == 0) {
        if (j == pX && i == pY) {
          u8g2.setFont(u8g2_font_6x12_t_symbols);
          u8g2.drawButtonUTF8(x, y, U8G2_BTN_INV, 0, 1, 1, "#");
        } else {
          u8g2.setFont(u8g2_font_6x12_t_symbols);
          u8g2.drawUTF8(x, y, "#");
        }
        continue;
      }

      if (strcmp(currentLayer[i][j], "<sym2>") == 0) {
        if (j == pX && i == pY) {
          u8g2.setFont(u8g2_font_6x12_t_symbols);
          u8g2.drawButtonUTF8(x, y, U8G2_BTN_INV, 0, 1, 1, "¬");
        } else {
          u8g2.setFont(u8g2_font_6x12_t_symbols);
          u8g2.drawUTF8(x, y, "¬");
        }
        continue;
      }

      if (strcmp(currentLayer[i][j], "<let>") == 0) {
        if (j == pX && i == pY) {
          u8g2.setFont(u8g2_font_6x12_t_symbols);
          u8g2.drawButtonUTF8(x, y, U8G2_BTN_INV, 0, 1, 1, "A");
        } else {
          u8g2.setFont(u8g2_font_6x12_t_symbols);
          u8g2.drawUTF8(x, y, "A");
        }
        continue;
      }

      if (strcmp(currentLayer[i][j], "<ok>") == 0) {
        if (j == pX && i == pY) {
          u8g2.setFont(u8g2_font_6x12_t_symbols);
          u8g2.drawButtonUTF8(x, y, U8G2_BTN_INV, 0, 1, 1, "✓");
        } else {
          u8g2.setFont(u8g2_font_6x12_t_symbols);
          u8g2.drawUTF8(x, y, "✓");
        }
        continue;
      }

      // # for symbols

      if (j == pX && i == pY)
        u8g2.drawButtonUTF8(x, y, U8G2_BTN_INV, 0, 1, 1,
                            (const char *)currentLayer[i][j]);
      else
        u8g2.drawStr(x, y, (const char *)currentLayer[i][j]);
    }
  }
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(2, 7, text.c_str());

  int cursorX = 2 + u8g2.getStrWidth(text.c_str()) + 2;
  u8g2.drawVLine(cursorX, 0, 9);

  u8g2.sendBuffer();
}

String keyboard(U8G2 u8g2, std::function<int(void)> doInput) {
  String text = "";
  int pointerX = 0;
  int pointerY = 0;

  int action;

  bool exit = false;
  while (!exit) {
    render_keyboard(pointerX, pointerY, text, u8g2);

    action = doInput();
    switch (action) {
    case ACTION_LEFT: {
      pointerX = min(9, pointerX + 1);
      break;
    }
    case ACTION_RIGHT: {
      pointerX = max(0, pointerX - 1);
      break;
    }
    case ACTION_DOWN: {
      pointerY = max(0, pointerY - 1);
      break;
    }
    case ACTION_UP: {
      pointerY = min(3, pointerY + 1);
      break;
    }
    default:
      break;
    }

    if (action == ACTION_SEL) {
      if (strcmp(currentLayer[pointerY][pointerX], "<caps>") == 0) {
        if (!caps)
          currentLayer = capitalLetters;
        if (caps)
          currentLayer = letters;

        caps = (!caps);
        goto skip_append;
      }

      if (strcmp(currentLayer[pointerY][pointerX], "<sym1>") == 0) {
        currentLayer = symbols1;
        goto skip_append;
      }

      if (strcmp(currentLayer[pointerY][pointerX], "<sym2>") == 0) {
        currentLayer = symbols2;
        goto skip_append;
      }

      if (strcmp(currentLayer[pointerY][pointerX], "<let>") == 0) {
        currentLayer = letters;
        goto skip_append;
      }

      if (strcmp(currentLayer[pointerY][pointerX], "<rm>") == 0) {
        text.remove(text.length() - 1);
        goto skip_append;
      }

      if (strcmp(currentLayer[pointerY][pointerX], "<ok>") == 0) {
        exit = true;
        goto skip_append;
      }

      if (caps) {
        String x = String(currentLayer[pointerY][pointerX]);
        x.toUpperCase();
        text += x;

        goto skip_append;
      }

      text += currentLayer[pointerY][pointerX];
    }
  skip_append:

    // Render the menu with the updated pointer position

    delay(150);
  }

  return text;
}
}; // namespace keyboard
