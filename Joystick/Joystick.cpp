/*
  Joystick.cpp

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

#include "Joystick.h"

#if defined(_USING_HID)

#define JOYSTICK_REPORT_ID 0x03
#define JOYSTICK_STATE_SIZE 13

static const uint8_t _hidReportDescriptor[] PROGMEM = {
  
	// Joystick
	0x05, 0x01,			      // USAGE_PAGE (Generic Desktop)
	0x09, 0x04,			      // USAGE (Joystick)
	0xa1, 0x01,			      // COLLECTION (Application)
	0x85, JOYSTICK_REPORT_ID, //   REPORT_ID (3)

	// 32 Buttons
	0x05, 0x09,			      //   USAGE_PAGE (Button)
	0x19, 0x01,			      //   USAGE_MINIMUM (Button 1)
	0x29, 0x20,			      //   USAGE_MAXIMUM (Button 32)
	0x15, 0x00,			      //   LOGICAL_MINIMUM (0)
	0x25, 0x01,			      //   LOGICAL_MAXIMUM (1)
	0x75, 0x01,			      //   REPORT_SIZE (1)
	0x95, 0x20,			      //   REPORT_COUNT (32)
	0x55, 0x00,			      //   UNIT_EXPONENT (0)
	0x65, 0x00,			      //   UNIT (None)
	0x81, 0x02,			      //   INPUT (Data,Var,Abs)

	// 8 bit Throttle and Steering
	0x05, 0x02,			      //   USAGE_PAGE (Simulation Controls)
	0x15, 0x00,			      //   LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00,	      //   LOGICAL_MAXIMUM (255)
	0xA1, 0x00,			      //   COLLECTION (Physical)
	0x09, 0xBB,			      //     USAGE (Throttle)
	0x09, 0xBA,			      //     USAGE (Steering)
	0x75, 0x08,			      //     REPORT_SIZE (8)
	0x95, 0x02,			      //     REPORT_COUNT (2)
	0x81, 0x02,			      //     INPUT (Data,Var,Abs)
	0xc0,				      //   END_COLLECTION

	// Two Hat switches (8 Positions)
	0x05, 0x01,			      //   USAGE_PAGE (Generic Desktop)
	0x09, 0x39,			      //   USAGE (Hat switch)
	0x15, 0x00,			      //   LOGICAL_MINIMUM (0)
	0x25, 0x07,			      //   LOGICAL_MAXIMUM (7)
	0x35, 0x00,			      //   PHYSICAL_MINIMUM (0)
	0x46, 0x3B, 0x01,	      //   PHYSICAL_MAXIMUM (315)
	0x65, 0x14,			      //   UNIT (Eng Rot:Angular Pos)
	0x75, 0x04,			      //   REPORT_SIZE (4)
	0x95, 0x01,			      //   REPORT_COUNT (1)
	0x81, 0x02,			      //   INPUT (Data,Var,Abs)
                              
	0x09, 0x39,			      //   USAGE (Hat switch)
	0x15, 0x00,			      //   LOGICAL_MINIMUM (0)
	0x25, 0x07,			      //   LOGICAL_MAXIMUM (7)
	0x35, 0x00,			      //   PHYSICAL_MINIMUM (0)
	0x46, 0x3B, 0x01,	      //   PHYSICAL_MAXIMUM (315)
	0x65, 0x14,			      //   UNIT (Eng Rot:Angular Pos)
	0x75, 0x04,			      //   REPORT_SIZE (4)
	0x95, 0x01,			      //   REPORT_COUNT (1)
	0x81, 0x02,			      //   INPUT (Data,Var,Abs)

	// X, Y, and Z Axis
	0x15, 0x00,			      //   LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00,	      //   LOGICAL_MAXIMUM (255)
	0x75, 0x08,			      //   REPORT_SIZE (8)
	0x09, 0x01,			      //   USAGE (Pointer)
	0xA1, 0x00,			      //   COLLECTION (Physical)
	0x09, 0x30,		          //     USAGE (x)
	0x09, 0x31,		          //     USAGE (y)
	0x09, 0x32,		          //     USAGE (z)
	0x09, 0x33,		          //     USAGE (rx)
	0x09, 0x34,		          //     USAGE (ry)
	0x09, 0x35,		          //     USAGE (rz)
	0x95, 0x06,		          //     REPORT_COUNT (6)
	0x81, 0x02,		          //     INPUT (Data,Var,Abs)
	0xc0,				      //   END_COLLECTION
                              
	0xc0				      // END_COLLECTION
};

Joystick_::Joystick_()
{
	// Setup HID report structure
	static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
	HID().AppendDescriptor(&node);
	
	// Initalize State
	xAxis = 0;
	yAxis = 0;
	zAxis = 0;
	xAxisRotation = 0;
	yAxisRotation = 0;
	zAxisRotation = 0;
	buttons = 0;
	throttle = 0;
	rudder = 0;
	hatSwitch[0] = -1;
	hatSwitch[1] = -1;
}

void Joystick_::begin(bool initAutoSendState)
{
	autoSendState = initAutoSendState;
	sendState();
}

void Joystick_::end()
{
}

void Joystick_::setButton(uint8_t button, uint8_t value)
{
	if (value == 0)
	{
		releaseButton(button);
	}
	else
	{
		pressButton(button);
	}
}
void Joystick_::pressButton(uint8_t button)
{
	bitSet(buttons, button);
	if (autoSendState) sendState();
}
void Joystick_::releaseButton(uint8_t button)
{
	bitClear(buttons, button);
	if (autoSendState) sendState();
}

void Joystick_::setThrottle(uint8_t value)
{
	throttle = value;
	if (autoSendState) sendState();
}
void Joystick_::setRudder(uint8_t value)
{
	rudder = value;
	if (autoSendState) sendState();
}

void Joystick_::setXAxis(int8_t value)
{
	xAxis = value;
	if (autoSendState) sendState();
}
void Joystick_::setYAxis(int8_t value)
{
	yAxis = value;
	if (autoSendState) sendState();
}
void Joystick_::setZAxis(int8_t value)
{
	zAxis = value;
	if (autoSendState) sendState();
}

void Joystick_::setXAxisRotation(int16_t value)
{
	xAxisRotation = value;
	if (autoSendState) sendState();
}
void Joystick_::setYAxisRotation(int16_t value)
{
	yAxisRotation = value;
	if (autoSendState) sendState();
}
void Joystick_::setZAxisRotation(int16_t value)
{
	zAxisRotation = value;
	if (autoSendState) sendState();
}

void Joystick_::setHatSwitch(int8_t hatSwitchIndex, int16_t value)
{
	hatSwitch[hatSwitchIndex % 2] = value;
	if (autoSendState) sendState();
}

void Joystick_::sendState()
{
	uint8_t data[JOYSTICK_STATE_SIZE];
	uint32_t buttonTmp = buttons;

	// Split 32 bit button-state into 4 bytes
	data[0] = buttonTmp & 0xFF;		
	buttonTmp >>= 8;
	data[1] = buttonTmp & 0xFF;
	buttonTmp >>= 8;
	data[2] = buttonTmp & 0xFF;
	buttonTmp >>= 8;
	data[3] = buttonTmp & 0xFF;

	data[4] = throttle;
	data[5] = rudder;

	// Calculate hat-switch values
	uint8_t convertedHatSwitch[2];
	for (int hatSwitchIndex = 0; hatSwitchIndex < 2; hatSwitchIndex++)
	{
		if (hatSwitch[hatSwitchIndex] < 0)
		{
			convertedHatSwitch[hatSwitchIndex] = 8;
		}
		else
		{
			convertedHatSwitch[hatSwitchIndex] = (hatSwitch[hatSwitchIndex] % 360) / 45;
		}
	}

	// Pack hat-switch states into a single byte
	data[6] = (convertedHatSwitch[1] << 4) | (B00001111 & convertedHatSwitch[0]);

	data[7] = xAxis + 127;
	data[8] = yAxis + 127;
	data[9] = zAxis + 127;

	data[10] = (xAxisRotation % 360) * 0.708;
	data[11] = (yAxisRotation % 360) * 0.708;
	data[12] = (zAxisRotation % 360) * 0.708;

	// HID().SendReport(Report number, array of values in same order as HID descriptor, length)
	HID().SendReport(JOYSTICK_REPORT_ID, data, JOYSTICK_STATE_SIZE);
}

Joystick_ Joystick;

#endif