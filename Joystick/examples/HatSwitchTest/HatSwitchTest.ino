// Simple example application that shows how to read four Arduino
// digital pins and map them to the USB Joystick library's hat switch.
//
// The digital pins 4, 5, 6, 7, 8, 9, 10, and 11 are grounded when 
// they are pressed.
//
// Pin Mappings:
//   4 - Hat Switch #0 UP
//   5 - Hat Switch #0 RIGHT
//   6 - Hat Switch #0 DOWN
//   7 - Hat Switch #0 LEFT
//   8 - Hat Switch #1 UP
//   9 - Hat Switch #1 RIGHT
//  10 - Hat Switch #1 DOWN
//  11 - Hat Switch #1 LEFT
//
// NOTE: This sketch file is for use with Arduino Leonardo and
//       Arduino Micro only.
//
// by Matthew Heironimus
// 2016-05-30
//--------------------------------------------------------------------

#include <Joystick.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_GAMEPAD, 0,
  JOYSTICK_DEFAULT_HATSWITCH_COUNT,
  false, false, false, false, false, false,
  false, false, false, false, false);

void setup() {
  
  // Initialize Button Pins
  for (int index = 4; index < 12; index++)
  {
    pinMode(index, INPUT_PULLUP);
  }

  // Initialize Joystick Library
  Joystick.begin();
}

// Last state of the pins
int lastButtonState[2][4] = {{0,0,0,0}, {0,0,0,0}};

void loop() {

  bool valueChanged[2] = {false, false};
  int currentPin = 4;

  // Read pin values
  for (int hatSwitch = 0; hatSwitch < 2; hatSwitch++)
  {
    for (int index = 0; index < 4; index++)
    {
      int currentButtonState = !digitalRead(currentPin++);
      if (currentButtonState != lastButtonState[hatSwitch][index])
      {
        valueChanged[hatSwitch] = true;
        lastButtonState[hatSwitch][index] = currentButtonState;
      }
    }
  }

  for (int hatSwitch = 0; hatSwitch < 2; hatSwitch++)
  {
    if (valueChanged[hatSwitch]) {
      
      if ((lastButtonState[hatSwitch][0] == 0)
        && (lastButtonState[hatSwitch][1] == 0)
        && (lastButtonState[hatSwitch][2] == 0)
        && (lastButtonState[hatSwitch][3] == 0)) {
          Joystick.setHatSwitch(hatSwitch, -1);
      }
      if (lastButtonState[hatSwitch][0] == 1) {
        Joystick.setHatSwitch(hatSwitch, 0);
      }
      if (lastButtonState[hatSwitch][1] == 1) {
        Joystick.setHatSwitch(hatSwitch, 90);
      }
      if (lastButtonState[hatSwitch][2] == 1) {
        Joystick.setHatSwitch(hatSwitch, 180);
      }
      if (lastButtonState[hatSwitch][3] == 1) {
        Joystick.setHatSwitch(hatSwitch, 270);
      }
      
    } // if the value changed
    
  } // for each hat switch

  delay(50);
}

