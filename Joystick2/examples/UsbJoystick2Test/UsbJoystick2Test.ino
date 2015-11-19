// Program used to test the Dual Simple USB Joystick object 
// on the Arduino Leonardo or Arduino Micro.
//
// Matthew Heironimus
// 2015-04-05
//------------------------------------------------------------

#include "Joystick2.h"

// Set to true to test "Auto Send" mode or false to test "Manual Send" mode.
//const bool testAutoSendMode = true;
const bool testAutoSendMode = false;

const unsigned long gcCycleDelta = 1000;
const unsigned long gcAnalogDelta = 25;
const unsigned long gcButtonDelta = 500;
unsigned long gNextTime = 0;
unsigned int gCurrentStep = 0;
int gJoystickId = 0;

void testSingleButtonPush(int joystickId, unsigned int button)
{
  if (button > 0)
  {
    Joystick[joystickId].releaseButton(button - 1);
  }
  if (button < 16)
  {
    Joystick[joystickId].pressButton(button);
  }
}

void testMultiButtonPush(int joystickId, unsigned int currentStep) 
{
  for (int button = 0; button < 16; button++)
  {
    if ((currentStep == 0) || (currentStep == 2))
    {
      if ((button % 2) == 0)
      {
        Joystick[joystickId].pressButton(button);
      } else if (currentStep != 2)
      {
        Joystick[joystickId].releaseButton(button);
      }
    } // if ((currentStep == 0) || (currentStep == 2))
    if ((currentStep == 1) || (currentStep == 2))
    {
      if ((button % 2) != 0)
      {
        Joystick[joystickId].pressButton(button);
      } else if (currentStep != 2)
      {
        Joystick[joystickId].releaseButton(button);
      }
    } // if ((currentStep == 1) || (currentStep == 2))
    if (currentStep == 3)
    {
      Joystick[joystickId].releaseButton(button);
    } // if (currentStep == 3)
  } // for (int button = 0; button < 32; button++)
}

void testXYAxis(int joystickId, unsigned int currentStep)
{
  if (currentStep < 255)
  {
    Joystick[joystickId].setXAxis(currentStep - 127);
    Joystick[joystickId].setYAxis(-127);
  } 
  else if (currentStep < 510)
  {
    Joystick[joystickId].setYAxis(currentStep - 255 - 127);
  }
  else if (currentStep < 765)
  {
    Joystick[joystickId].setXAxis(127 - (currentStep - 510));
  }
  else if (currentStep < 1020)
  {
    Joystick[joystickId].setYAxis(127 - (currentStep - 765));
  }
  else if (currentStep <= 1020 + 127)
  {
    Joystick[joystickId].setXAxis(currentStep - 1020 - 127);
    Joystick[joystickId].setYAxis(currentStep - 1020 - 127);
  }
}

void setup() {
  if (testAutoSendMode)
  {
    Joystick[0].begin();
    Joystick[1].begin();
  }
  else
  {
    Joystick[0].begin(false);
    Joystick[1].begin(false);
  }
  
  pinMode(A0, INPUT_PULLUP);
  pinMode(13, OUTPUT);
}

void loop() {

  // System Disabled
  if (digitalRead(A0) != 0)
  {
    digitalWrite(13, 0);
    return;
  }

  // Turn indicator light on.
  digitalWrite(13, 1);
  
  if (millis() >= gNextTime)
  {
   
    if (gCurrentStep < 17)
    {
      gNextTime = millis() + gcButtonDelta;
      testSingleButtonPush(gJoystickId, gCurrentStep);
    } 
    else if (gCurrentStep < (17 + 4))
    {
      gNextTime = millis() + gcButtonDelta;
      testMultiButtonPush(gJoystickId, gCurrentStep - 17);
    }
    else if (gCurrentStep < (17 + 4 + 1024 + 128))
    {
      gNextTime = millis() + gcAnalogDelta;
      testXYAxis(gJoystickId, gCurrentStep - (17 + 4));
    }
    
    if (testAutoSendMode == false)
    {
      Joystick[gJoystickId].sendState();
    }
    
    gCurrentStep++;
    if (gCurrentStep == (17 + 4 + 1024 + 128))
    {
      gNextTime = millis() + gcCycleDelta;
      gCurrentStep = 0;
      
      gJoystickId = (gJoystickId == 0 ? 1 : 0);
    }
  }
}

