// Program used to test the driving simulator functions on 
// the USB Joystick object on the Arduino Leonardo or 
// Arduino Micro.
//
// Matthew Heironimus
// 2016-05-29   Original version.
//------------------------------------------------------------

#include "Joystick.h"

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_MULTI_AXIS, 4, 0,
  false, false, false, false, false, false,
  false, false, true, true, true);

// Set to true to test "Auto Send" mode or false to test "Manual Send" mode.
//const bool testAutoSendMode = true;
const bool testAutoSendMode = false;

const unsigned long gcCycleDelta = 1000;
const unsigned long gcButtonDelta = 500;
const unsigned long gcAnalogDelta = 25;
unsigned long gNextTime = 0;
unsigned int gCurrentStep = 0;

void testSingleButtonPush(unsigned int button)
{
  if (button > 0)
  {
    Joystick.releaseButton(button - 1);
  }
  if (button < 4)
  {
    Joystick.pressButton(button);
  }
}

void testMultiButtonPush(unsigned int currentStep) 
{
  for (int button = 0; button < 4; button++)
  {
    if ((currentStep == 0) || (currentStep == 2))
    {
      if ((button % 2) == 0)
      {
        Joystick.pressButton(button);
      } else if (currentStep != 2)
      {
        Joystick.releaseButton(button);
      }
    } // if ((currentStep == 0) || (currentStep == 2))
    if ((currentStep == 1) || (currentStep == 2))
    {
      if ((button % 2) != 0)
      {
        Joystick.pressButton(button);
      } else if (currentStep != 2)
      {
        Joystick.releaseButton(button);
      }
    } // if ((currentStep == 1) || (currentStep == 2))
    if (currentStep == 3)
    {
      Joystick.releaseButton(button);
    } // if (currentStep == 3)
  } // for (int button = 0; button < 32; button++)
}

void testAcceleratorBrake(int value)
{
  Joystick.setAccelerator(value);
  Joystick.setBrake(260 - value);
}

void testSteering(int value)
{
  if (value < 300) {
    Joystick.setSteering(value);
  } else {
    Joystick.setSteering(600 - value);
  }
}

void setup() {

  Joystick.setAcceleratorRange(0, 260);
  Joystick.setBrakeRange(0, 260);
  Joystick.setSteeringRange(0, 300);
  
  if (testAutoSendMode)
  {
    Joystick.begin();
  }
  else
  {
    Joystick.begin(false);
  }
  
  pinMode(A0, INPUT_PULLUP);
  pinMode(13, OUTPUT);
}

void loop() {

  // System Disabled
  if (digitalRead(A0) != 0)
  {
    // Turn indicator light off.
    digitalWrite(13, 0);
    return;
  }

  // Turn indicator light on.
  digitalWrite(13, 1);
  
  if (millis() >= gNextTime)
  {
   
    if (gCurrentStep < 4)
    {
      gNextTime = millis() + gcButtonDelta;
      testSingleButtonPush(gCurrentStep);
    } 
    else if (gCurrentStep < 9)
    {
      gNextTime = millis() + gcButtonDelta;
      testMultiButtonPush(gCurrentStep - 5);
    }
    else if (gCurrentStep < (9 + 260))
    {
      gNextTime = millis() + gcAnalogDelta;
      testAcceleratorBrake(gCurrentStep - 9);
    }
    else if (gCurrentStep < (9 + 260 + 600))
    {
      gNextTime = millis() + gcAnalogDelta;
      testSteering(gCurrentStep - (9 + 260));
    }
    
    if (testAutoSendMode == false)
    {
      Joystick.sendState();
    }
    
    gCurrentStep++;
    if (gCurrentStep >= (9 + 260 + 600))
    {
      gNextTime = millis() + gcCycleDelta;
      gCurrentStep = 0;
    }
  }
}

