// Based on code from : https://github.com/Dak0r/Chinese-SimRacing-14Bit-Handbrake-Custom-Firmware-Arduino-Sketch
// Using : https://github.com/MHeironimus/ArduinoJoystickLibrary

#define DEBUG 0 // uncomment for debug output

#define AXIS_RESOLUTION   256 //8 Bits axis resolution
#define PIN_HALL_SENSOR   A2

#include <Joystick.h>

int minValue = 453;
int maxValue = 501;
int deadZone = 3;

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   3,                      // Button Count
                   0,                      // Hat Switch Count
                   false, false, true,     // X and Y, but no Z Axis
                   false, false, false,    // No Rx, Ry, or Rz
                   false, false,           // No rudder or throttle
                   false, false, false);   // No accelerator, brake, or steering

void setup() {

#if DEBUG
  Serial.begin(9600);
#endif

  // Initialize Button Pins
  pinMode(PIN_HALL_SENSOR, INPUT);

  // Initialize Joystick Library
  Joystick.begin();
  Joystick.setZAxisRange(0, AXIS_RESOLUTION);
}

void loop() {

  int hallSensorValue = analogRead(PIN_HALL_SENSOR);

  //self calibration:
  if (hallSensorValue < minValue) {
    minValue = hallSensorValue;
  } else if (hallSensorValue > maxValue) {
    maxValue = hallSensorValue;
  }

  int zAxisValue = ((hallSensorValue - minValue - deadZone) / (float)(maxValue - minValue - (2 * deadZone))) * AXIS_RESOLUTION;

  // Remove invalid Values
  if (zAxisValue > AXIS_RESOLUTION) {
    zAxisValue = AXIS_RESOLUTION;
  }
  if (zAxisValue < 0) {
    zAxisValue = 0;
  }

  // Invert Axis
  zAxisValue = AXIS_RESOLUTION - zAxisValue;

  // Button mappings
  if (zAxisValue > 32) {
    Joystick.setButton(0, 1);
  }
  else {
    Joystick.setButton(0, 0);
  }

  if (zAxisValue > 128) {
    Joystick.setButton(1, 1);
  }
  else {
    Joystick.setButton(1, 0);
  }

  if (zAxisValue > 224) {
    Joystick.setButton(2, 1);
  }
  else {
    Joystick.setButton(2, 0);
  }

  // Set value
  Joystick.setZAxis(zAxisValue);

#if DEBUG
  Serial.print(hallSensorValue);
  Serial.print(" - ");
  Serial.println(zAxisValue);
#endif
  Joystick.setZAxis(zAxisValue);


  delay(10);
}
