# OLED Keyboard
![image](https://github.com/user-attachments/assets/b24e4b5d-25f7-41ef-b7be-d8860d228fa3)

A tiny keyboard library for 128x64 OLEDs. It is not dependent on any one form of input (i.e. you can use different input modes)
It is based on the U8G2 library

## Usage
```cpp
#include <U8g2lib.h>
#include "keyboard.h"

U8G2 u8g2();

int inputFunc() {
  // Do some input stuff and return one of the following:
  //    - ACTION_LEFT 
  //    - ACTION_RIGHT
  //    - ACTION_UP 
  //    - ACTION_DOWN 
  //    - ACTION_SEL 

}

void doKeyboard() {
  String text = keyboard::keyboard(u8g2, inputFunc);
}
```

To use it in your project. Download [this](https://github.com/ironlungx/oledKeyboard/blob/main/src/keyboard.h) file in your project.
On platformIO you need to put it in the `include` directory

wokwi example [here](https://wokwi.com/projects/404493837770319873)
