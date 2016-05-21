/*
  Joystick.h

  Copyright (c) 2015, Matthew Heironimus

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef JOYSTICK_h
#define JOYSTICK_h

#include "DynamicHID.h"

#if ARDUINO < 10606
#error The Joystick library requires Arduino IDE 1.6.6 or greater. Please update your IDE.
#endif

#if !defined(USBCON)
#error The Joystick library can only be used with a USB MCU (e.g. Arduino Leonardo, Arduino Micro, etc.).
#endif

#if !defined(_USING_DYNAMIC_HID)

#warning "Using legacy HID core (non pluggable)"

#else

//================================================================================
//================================================================================
//  Joystick (Gamepad)

#define JOYSTICK_DEFAULT_REPORT_ID    0x03
#define JOYSTICK_DEFAULT_BUTTON_COUNT   32

class Joystick_
{
private:

    // Joystick State
	int16_t	                 _xAxis;
	int16_t	                 _yAxis;
	int16_t	                 _zAxis;
	int16_t	                 _xAxisRotation;
	int16_t	                 _yAxisRotation;
	int16_t	                 _zAxisRotation;
	int16_t                  _throttle;
	int16_t                  _rudder;
	int16_t	                 _hatSwitch[2];
    uint8_t                 *_buttonValues;

    // Joystick Settings
    bool                     _autoSendState;
    uint8_t                  _buttonCount;
    int                      _buttonValuesArraySize;

	DynamicHIDSubDescriptor *_node;
	uint8_t                  _hidReportId;
	int                      _hidReportSize; 

public:
	Joystick_(uint8_t hidReportId = JOYSTICK_DEFAULT_REPORT_ID,
        uint8_t buttonCount = JOYSTICK_DEFAULT_BUTTON_COUNT);

	void begin(bool initAutoSendState = true);
	void end();

	void setXAxis(int16_t value);
	void setYAxis(int16_t value);
	void setZAxis(int16_t value);

	void setXAxisRotation(int16_t value);
	void setYAxisRotation(int16_t value);
	void setZAxisRotation(int16_t value);

	void setButton(uint8_t button, uint8_t value);
	void pressButton(uint8_t button);
	void releaseButton(uint8_t button);

	void setThrottle(int16_t value);
	void setRudder(int16_t value);

	void setHatSwitch(int8_t hatSwitch, int16_t value);

	void sendState();
};

#endif
#endif
