/*
  Joystick3.h

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

#include "HID.h"

#if ARDUINO < 10606
#error The Joystick3 library requires Arduino IDE 1.6.6 or greater. Please update your IDE.
#endif

#if ARDUINO > 10606
#if !defined(USBCON)
#error The Joystick3 library can only be used with a USB MCU (e.g. Arduino Leonardo, Arduino Micro, etc.).
#endif
#endif

#if !defined(_USING_HID)

#warning "Using legacy HID core (non pluggable)"

#else

//================================================================================
//================================================================================
//  Joystick (Gamepad)

class Joystick_
{
private:
	uint8_t  joystickId;
	bool     autoSendState;
	int8_t	 xAxis;
	int8_t	 yAxis;
	uint16_t buttons;

public:
	Joystick_(uint8_t initJoystickId);

	void begin(bool initAutoSendState = true);
	void end();

	void setXAxis(int8_t value);
	void setYAxis(int8_t value);

	void setButton(uint8_t button, uint8_t value);
	void pressButton(uint8_t button);
	void releaseButton(uint8_t button);

	void sendState();
};
extern Joystick_ Joystick[3];

#endif
#endif
