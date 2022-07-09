#include <Joystick.h>

const uint8_t buttonCount = 7;
Joystick_ controller(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, buttonCount,
                     0, true, true, false,
                     false, false, false,
                     false, false, false,
                     false, false);

int const BTN_A_PIN = 2;
int const BTN_B_PIN = 3;
int const BTN_C_PIN = 4;
int const BTN_D_PIN = 5;
int const BTN_E_PIN = 6;
int const BTN_F_PIN = 7;
int const BTN_K_PIN = 8;
int const AXIS_X_PIN = A0;
int const AXIS_Y_PIN = A1;

int const buttonPins[buttonCount] = {
  BTN_A_PIN,
  BTN_B_PIN,
  BTN_C_PIN,
  BTN_D_PIN,
  BTN_E_PIN,
  BTN_F_PIN,
  BTN_K_PIN
};
int lastButtonValue[buttonCount];
int lastXAxisValue = -1;
int lastYAxisValue = -1;

void setup()
{
  controller.setYAxisRange(0, 1023);
  controller.setYAxisRange(1023, 0);
  controller.begin(false);

  for (int i = 0; i < buttonCount; i++)
  {
    pinMode(buttonPins[i], INPUT_PULLUP);
    lastButtonValue[i] = -1;
  }

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  bool sendUpdate = false;
  for (int i = 0; i < buttonCount; i++)
  {
    const int buttonValue = digitalRead(buttonPins[i]);

    if (buttonValue != lastButtonValue[i])
    {
      controller.setButton(i, !buttonValue);
      lastButtonValue[i] = buttonValue;
      sendUpdate = true;
    }
  }

  const int currentXAxisValue = analogRead(AXIS_X_PIN);
  if (currentXAxisValue != lastXAxisValue)
  {
    controller.setXAxis(currentXAxisValue);
    lastXAxisValue = currentXAxisValue;
    sendUpdate = true;
  }

  const int currentYAxisValue = analogRead(AXIS_Y_PIN);
  if (currentYAxisValue != lastYAxisValue)
  {
    controller.setYAxis(currentYAxisValue);
    lastYAxisValue = currentYAxisValue;
    sendUpdate = true;
  }

  if (sendUpdate)
  {
    controller.sendState();
  }
  delay(50);
}
