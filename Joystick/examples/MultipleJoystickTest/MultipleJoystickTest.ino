// Program used to test using the Arduino Joystick Library 
// to create multiple Joystick objects on a single Arduino 
// Leonardo or Arduino Micro.
//
// Each joystick has a unique configuration.
//
// Matthew Heironimus
// 2016-05-13
//------------------------------------------------------------
#include <Joystick.h>

#define JOYSTICK_COUNT 4

Joystick_ Joystick[JOYSTICK_COUNT] = {
  Joystick_(0x03, JOYSTICK_TYPE_GAMEPAD, 4, 2, true, true, false, false, false, false, false, false, false, false, false),
  Joystick_(0x04, JOYSTICK_TYPE_JOYSTICK, 8, 1, true, true, true, true, false, false, false, false, false, false, false),
  Joystick_(0x05, JOYSTICK_TYPE_MULTI_AXIS, 16, 0, false, true, false, true, false, false, true, true, false, false, false),
  Joystick_(0x06, JOYSTICK_TYPE_MULTI_AXIS, 32, 1, true, true, false, true, true, false, false, false, true, true, true)
};

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

  for (int index = 0; index < JOYSTICK_COUNT; index++)
  {
    Joystick[index].setXAxisRange(-127, 127);
    Joystick[index].setYAxisRange(-127, 127);
  
    if (testAutoSendMode)
    {
      Joystick[index].begin();
    }
    else
    {
      Joystick[index].begin(false);
    }
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
      
      if (++gJoystickId >= JOYSTICK_COUNT)
      {
        gJoystickId = 0;
      }
    }
  }
}
