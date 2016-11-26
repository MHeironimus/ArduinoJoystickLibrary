// Simple example application that shows how to read four Arduino
// digital pins and map them to buttons on a joystick or keys on a 
// keyboard uisng the Arduino Joystick and Keyboard libraries.
//
// The digital pins 9, 10, 11, and 12 are grounded when they are pressed.
//
// NOTE: This sketch file is for use with Arduino Leonardo and
//       Arduino Micro only.
//
// Pin  9 = Joystick Button 0 
// Pin 10 = Joystick Button 1
// Pin 11 = 1 key on the Keyboard 
// Pin 12 = 2 key on the Keyboard
//
// by Matthew Heironimus
// 2016-05-13
//--------------------------------------------------------------------

#include <Keyboard.h>
#include <Joystick.h>

Joystick_ Joystick;

void setup() {
  // Initialize Button Pins
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);

  // Initialize Joystick Library
  Joystick.begin();
}

// Constant that maps the phyical pin to the joystick button.
const int pinToButtonMap = 9;

// Last state of the button
int lastButtonState[4] = {0,0,0,0};

void loop() {

  // Read pin values
  for (int index = 0; index < 4; index++)
  {
    int currentButtonState = !digitalRead(index + pinToButtonMap);
    if (currentButtonState != lastButtonState[index])
    {
      if (index < 2) {
        Joystick.setButton(index, currentButtonState);
        lastButtonState[index] = currentButtonState;
      } else {
        if (currentButtonState) {
          Keyboard.write(47 + index);
          delay(500);
        }
      }
    }
  }

  delay(100);
}

