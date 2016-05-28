### WARNING: This branch is currently in development and is not stable at this time.

# Arduino Joystick Library
#### Version 2.0.0 (in development)
This library can be used with Arduino IDE 1.6.6 (or above) to add one or more joysticks (or gamepads) to the list of HID devices an Arduino Leonardo or Arduino Micro (or any Arduino clone that is based on the ATmega32u4) can support. This will not work with Arduino IDE 1.6.5 (or below).

##Installation Instructions
Copy the Joystick folder to the Arduino libraries folder (typically %userprofile%\Documents\Arduino\libraries). The library (or libraries) should now appear in the Arduino IDE list of libraries.

##Examples
The following example Arduino sketch files are included in this library:

- JoystickTest - Simple test of the Joystick library. Exercises all of the Joystick library functions when pin A0 is grounded.
- MultipleJoystickTest - Creates 4 Joysticks using the library and exercises the first 16 buttons and X and Y axis when pin A0 is grounded.
- JoystickButton - Creates a Joystick and maps pin 9 to button 0, pin 10 to button 1, pin 11 to button 2, and pin 12 to button 3.
- JoystickKeyboard - Creates a Joystick and a Keyboard. Maps pin 9 to Joystick Button 0, pin 10 to Joystick Button 1, pin 11 to Keyboard key 1, and pin 12 to Keyboard key 2.  

###Simple example

	#include <Joystick.h>
	
	Joystick_ Joystick;
	
	void setup() {
	  // Initialize Button Pins
	  pinMode(9, INPUT_PULLUP);
	
	  // Initialize Joystick Library
	  Joystick.begin();
	}
	
	// Constant that maps the phyical pin to the joystick button.
	const int pinToButtonMap = 9;
	
	// Last state of the button
	int lastButtonState = 0;
	
	void loop() {
	
	  // Read pin values
      int currentButtonState = !digitalRead(pinToButtonMap);
	  if (currentButtonState != lastButtonState)
	  {
	    Joystick.setButton(0, currentButtonState);
	    lastButtonState = currentButtonState;
	  }
	
	  delay(50);
	}

##Joystick Library API
The following API is available if the Joystick library in included in a sketch file.

### Joystick\_(...)
Constructor used to initialize and setup the Joystick. The following optional parameters are available:

- `uint8_t hidReportId` - Default: `0x03` - Indicates what the joystick's HID report ID should be. This value must be unique if you are creating multiple instances of Joystick. Do not use `0x01` or `0x02` as they are used by the built-in Arduino Keyboard and Mouse libraries.
- `uint8_t buttonCount` - Default: `32` - Indicates how many buttons will be available on the joystick.
- `bool includeXAxis` - Default: `true` - Indicates if the X Axis is available on the joystick.
- `bool includeYAxis` - Default: `true` - Indicates if the Y Axis is available on the joystick.
- `bool includeZAxis` - Default: `true` - Indicates if the Z Axis (in some situations this is the right X Axis) is available on the joystick.
- `bool includeRxAxis` - Default: `true` - Indicates if the X Axis Rotation (in some situations this is the right Y Axis) is available on the joystick.
- `bool includeRyAxis` - Default: `true` - Indicates if the Y Axis Rotation is available on the joystick.
- `bool includeRzAxis` - Default: `true` - Indicates if the Z Axis Rotation is available on the joystick.

The following constants define the default values for the constructor parameter's listed above:

- `JOYSTICK_DEFAULT_REPORT_ID` is set to `0x03`
- `JOYSTICK_DEFAULT_BUTTON_COUNT` is set to `32`. 
		
### Joystick.begin(bool initAutoSendState)
Starts emulating a game controller connected to a computer. By default all methods update the game controller state immediately. If initAutoSendState is set to false, the Joystick.sendState method must be called to update the game controller state.

### Joystick.end()
Stops the game controller emulation to a connected computer.

### Joystick.setXAxisRange(int16_t minimum, int16_t maximum)
Sets the range of values that will be used for the X axis. Default: `0` to `1023`

### Joystick.setXAxis(byte value)
Sets the X axis value. See `setXAxisRange` for the range.

### Joystick.setYAxisRange(int16_t minimum, int16_t maximum)
Sets the range of values that will be used for the Y axis. Default: `0` to `1023`

### Joystick.setYAxis(byte value)
Sets the Y axis value. See `setYAxisRange` for the range.

### Joystick.setZAxisRange(int16_t minimum, int16_t maximum)
Sets the range of values that will be used for the Z axis. Default: `0` to `1023`

### Joystick.setZAxis(byte value)
Sets the Z axis value. See `setZAxisRange` for the range.

### Joystick.setRxAxisRange(int16_t minimum, int16_t maximum)
Sets the range of values that will be used for the X axis rotation. Default: `0` to `1023`

### Joystick.setRxAxis(int value)
Sets the X axis rotation value. See `setRxAxisRange` for the range.

### Joystick.setRyAxisRange(int16_t minimum, int16_t maximum)
Sets the range of values that will be used for the Y axis rotation. Default: `0` to `1023`

### Joystick.setRyAxis(int value)
Sets the Y axis rotation value. See `setRyAxisRange` for the range.

### Joystick.setRzAxisRange(int16_t minimum, int16_t maximum)
Sets the range of values that will be used for the Z axis rotation. Default: `0` to `1023`

### Joystick.setRzAxis(int value)
Sets the Z axis rotation value. See `setRzAxisRange` for the range.

###Joystick.setButton(byte button, byte value)
Sets the state (`0` or `1`) of the specified button (range: `0` - (`buttonCount - 1`)). The button is the 0-based button number (i.e. button #1 is 0, button #2 is 1, etc.). The value is 1 if the button is pressed and 0 if the button is released.

###Joystick.pressButton(byte button)
Press the indicated button (range: `0` - (`buttonCount - 1`)). The button is the 0-based button number (i.e. button #1 is 0, button #2 is 1, etc.).

###Joystick.releaseButton(byte button)
Release the indicated button (range: `0` - (`buttonCount - 1`)). The button is the 0-based button number (i.e. button #1 is 0, button #2 is 1, etc.).

###Joystick.setThrottle(byte value)
Sets the throttle value. Range 0 to 255.

###Joystick.setRudder(byte value)
Sets the rudder value. Range 0 to 255.

###Joystick.setHatSwitch(byte hatSwitch, int value)
Sets the value of the specified hat switch. The hatSwitch is 0-based (i.e. hat switch #1 is 0 and hat switch #2 is 1). The value is from 0° to 360°, but in 45° increments. Any value less than 45° will be rounded down (i.e. 44° is rounded down to 0°, 89° is rounded down to 45°, etc.). Set the value to -1 to release the hat switch.

###Joystick.sendState()
Sends the updated joystick state to the host computer. Only needs to be called if AutoSendState is false (see Joystick.begin for more details).

##Testing Details
I have used this library to make an Arduino appear as the following:

- 1 joystick / gamepad
- 2 joysticks / gamepads
- 3 joysticks / gamepads
- 4 joysticks / gamepads

I have tested with 1 - 32 buttons.

I have tested this library using the following Arduino IDE Versions:

- 1.6.6
- 1.6.7
- 1.6.8

I have tested this library with the following boards:

- Arduino Leonardo
- Arduino Micro

Others have tested this library with the following boards:
- Arduino Due
- SparkFun Pro Micro

(as of 2016-05-23)
