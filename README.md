# Arduino Joystick Library
#### Version 1.0.1

Arduino IDE 1.6.6 (or above) library that adds a joystick to the list of HID devices an Arduino Leonardo or Arduino Micro (or any Arduino clone that is based on the ATmega32u4) can support. This will not work with Arduino IDE 1.6.5 (or below).

The lastest version of this library can be found at [https://github.com/MHeironimus/ArduinoJoystickLibrary/tree/version-2.0](https://github.com/MHeironimus/ArduinoJoystickLibrary/tree/version-2.0).

##Support for one, two, or three joysticks
This library comes in three flavors:
- Joystick - adds a single joystick that contains an X, Y, and Z axis (including rotation), 32 buttons, 2 hat switches, a throttle, and a rudder.
- Joystick2 - adds two simple joysticks that contain an X and Y axis and 16 buttons.
- Joystick3 - adds three simple joysticks that contain an X and Y axis and 16 buttons.

##Installation Instructions
Copy one or more of the folders (Joystick, Joystick2, and Joystick3) to the Arduino libraries folder (typically %userprofile%\Documents\Arduino\libraries). The library (or libraries) should now appear in the Arduino IDE list of libraries.

##Joystick Library API
The following API is available if the Joystick library in included in a sketch file.

###Joystick.begin(bool initAutoSendState)
Starts emulating a game controller connected to a computer. By default all methods update the game controller state immediately. If initAutoSendState is set to false, the Joystick.sendState method must be called to update the game controller state.

###Joystick.end()
Stops the game controller emulation to a connected computer.

###Joystick.setXAxis(byte value)
Sets the X axis value. Range -127 to 127 (0 is center).

###Joystick.setYAxis(byte value)
Sets the Y axis value. Range -127 to 127 (0 is center).

###Joystick.setZAxis(byte value)
Sets the Z axis value. Range -127 to 127 (0 is center).

###Joystick.setXAxisRotation(int value)
Sets the X axis rotation value. Range 0° to 360°.

###Joystick.setYAxisRotation(int value)
Sets the Y axis rotation value. Range 0° to 360°.

###Joystick.setZAxisRotation(int value)
Sets the Z axis rotation value. Range 0° to 360°.

###Joystick.setButton(byte button, byte value)
Sets the state (0 or 1) of the specified button (0 - 31). The button is the 0-based button number (i.e. button #1 is 0, button #2 is 1, etc.). The value is 1 if the button is pressed and 0 if the button is released.

###Joystick.pressButton(byte button)
Press the indicated button (0 - 31). The button is the 0-based button number (i.e. button #1 is 0, button #2 is 1, etc.).

###Joystick.releaseButton(byte button)
Release the indicated button (0 - 31). The button is the 0-based button number (i.e. button #1 is 0, button #2 is 1, etc.).

###Joystick.setThrottle(byte value)
Sets the throttle value. Range 0 to 255.

###Joystick.setRudder(byte value)
Sets the rudder value. Range 0 to 255.

###Joystick.setHatSwitch(byte hatSwitch, int value)
Sets the value of the specified hat switch. The hatSwitch is 0-based (i.e. hat switch #1 is 0 and hat switch #2 is 1). The value is from 0° to 360°, but in 45° increments. Any value less than 45° will be rounded down (i.e. 44° is rounded down to 0°, 89° is rounded down to 45°, etc.). Set the value to -1 to release the hat switch.

###Joystick.sendState()
Sends the updated joystick state to the host computer. Only needs to be called if AutoSendState is false (see Joystick.begin for more details).

##Joystick2 and Joystick3 Library API
The following API is available if the Joystick2 or Joystick3 library in included in a sketch file.

The joystickIndex is 0-based (i.e. the first game controller has a joystickIndex of 0, the second has a joystickIndex of 1, and the third has a joystickIndex of 2).

###Joystick[joystickIndex].begin(bool initAutoSendState)
Starts emulating a game controller connected to a computer. By default all methods update the game controller state immediately. If initAutoSendState is set to false, the Joystick[joystickIndex].sendState method must be called to update the game controller state.

###Joystick[joystickIndex].end()
Stops the game controller emulation to a connected computer.

###Joystick[joystickIndex].setXAxis(byte value)
Sets the X axis value. Range -127 to 127 (0 is center).

###Joystick[joystickIndex].setYAxis(byte value)
Sets the Y axis value. Range -127 to 127 (0 is center).

###Joystick[joystickIndex].setButton(byte button, byte value)
Sets the state (0 or 1) of the specified button (0 - 15). The button is the 0-based button number (i.e. button #1 is 0, button #2 is 1, etc.). The value is 1 if the button is pressed and 0 if the button is released.

###Joystick[joystickIndex].pressButton(byte button)
Press the indicated button (0 - 15). The button is the 0-based button number (i.e. button #1 is 0, button #2 is 1, etc.).

###Joystick[joystickIndex].releaseButton(byte button)
Release the indicated button (0 - 15). The button is the 0-based button number (i.e. button #1 is 0, button #2 is 1, etc.).

###Joystick[joystickIndex].sendState()
Sends the updated joystick state to the host computer. Only needs to be called if AutoSendState is false (see Joystick[joystickIndex].begin for more details).
