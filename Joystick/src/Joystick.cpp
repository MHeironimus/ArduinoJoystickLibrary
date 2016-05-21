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

#if defined(_USING_DYNAMIC_HID)

//#define JOYSTICK_STATE_SIZE 13
//#define JOYSTICK_STATE_SIZE 4

#define JOYSTICK_REPORT_ID_INDEX 7

static const uint8_t _hidReportDescriptor[] PROGMEM = {
  
	// Joystick
	0x05, 0x01,			              // USAGE_PAGE (Generic Desktop)
	0x09, 0x04,			              // USAGE (Joystick)
	0xa1, 0x01,			              // COLLECTION (Application)
	0x85, JOYSTICK_DEFAULT_REPORT_ID, //   REPORT_ID (Default: 3)

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

Joystick_::Joystick_(uint8_t hidReportId,
    uint8_t buttonCount)
{
    // Set the USB HID Report ID
    _hidReportId = hidReportId;

    // Save Joystick Settings
    _buttonCount = buttonCount;

    // Build Joystick HID Report Description
	uint8_t buttonsInLastByte = _buttonCount % 8;
	uint8_t buttonPaddingBits = 0;
	if (buttonsInLastByte > 0)
	{
		buttonPaddingBits = 8 - buttonsInLastByte;
	}

	// TODO: Figure out what the max for this could be and set it here...
    uint8_t *customHidReportDescriptor = new uint8_t[100];
    int hidReportDescriptorSize = 0;

    // USAGE_PAGE (Generic Desktop)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x05;
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // USAGE (Joystick)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x04;

    // COLLECTION (Application)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0xa1;
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // REPORT_ID (Default: 3)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x85;
    customHidReportDescriptor[hidReportDescriptorSize++] = _hidReportId;

    // USAGE_PAGE (Button)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x05;
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x09;

    // USAGE_MINIMUM (Button 1)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x19;
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // USAGE_MAXIMUM (Button 32)            
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x29;
    customHidReportDescriptor[hidReportDescriptorSize++] = _buttonCount;

    // LOGICAL_MINIMUM (0)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x15;
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

    // LOGICAL_MAXIMUM (1)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x25;
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // REPORT_SIZE (1)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // REPORT_COUNT (# of buttons)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
    customHidReportDescriptor[hidReportDescriptorSize++] = _buttonCount;

    // UNIT_EXPONENT (0)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x55;
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

    // UNIT (None)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x65;
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

    // INPUT (Data,Var,Abs)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x02;

	if (buttonPaddingBits > 0) {
		
		// REPORT_SIZE (1)
		customHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
		customHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

		// REPORT_COUNT (# of padding bits)
		customHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
		customHidReportDescriptor[hidReportDescriptorSize++] = buttonPaddingBits;
				
		// INPUT (Const,Var,Abs)
		customHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
		customHidReportDescriptor[hidReportDescriptorSize++] = 0x03;
	}

    // END_COLLECTION
    customHidReportDescriptor[hidReportDescriptorSize++] = 0xc0;

	// Create a copy of the HID Report Descriptor template
//	memcpy_P(customHidReportDescriptor, _hidReportDescriptor, hidReportDescriptorSize);
	
	// Register HID Report Description
	_node = new DynamicHIDSubDescriptor(customHidReportDescriptor, hidReportDescriptorSize, false);
	DynamicHID().AppendDescriptor(_node);
	
    // Setup Joystick State
    _buttonValuesArraySize = _buttonCount / 8;
    if ((_buttonCount % 8) > 0) {
        _buttonValuesArraySize++;
    }
    _buttonValues = new uint8_t[_buttonValuesArraySize];

	// Calculate HID Report Size
	_hidReportSize = _buttonValuesArraySize;

	// Initalize Joystick State
	_xAxis = 0;
	_yAxis = 0;
	_zAxis = 0;
	_xAxisRotation = 0;
	_yAxisRotation = 0;
	_zAxisRotation = 0;
	_throttle = 0;
	_rudder = 0;
	_hatSwitch[0] = -1;
	_hatSwitch[1] = -1;
    for (int index = 0; index < _buttonValuesArraySize; index++)
    {
        _buttonValues[index] = 0;
    }
}

void Joystick_::begin(bool initAutoSendState)
{
	_autoSendState = initAutoSendState;
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
    if (button >= _buttonCount) return;

    int index = button / 8;
    int bit = button % 8;

	bitSet(_buttonValues[index], bit);
	if (_autoSendState) sendState();
}
void Joystick_::releaseButton(uint8_t button)
{
    if (button >= _buttonCount) return;

    int index = button / 8;
    int bit = button % 8;

    bitClear(_buttonValues[index], bit);
	if (_autoSendState) sendState();
}

void Joystick_::setThrottle(int16_t value)
{
	_throttle = value;
	if (_autoSendState) sendState();
}
void Joystick_::setRudder(int16_t value)
{
	_rudder = value;
	if (_autoSendState) sendState();
}

void Joystick_::setXAxis(int16_t value)
{
	_xAxis = value;
	if (_autoSendState) sendState();
}
void Joystick_::setYAxis(int16_t value)
{
	_yAxis = value;
	if (_autoSendState) sendState();
}
void Joystick_::setZAxis(int16_t value)
{
	_zAxis = value;
	if (_autoSendState) sendState();
}

void Joystick_::setXAxisRotation(int16_t value)
{
	_xAxisRotation = value;
	if (_autoSendState) sendState();
}
void Joystick_::setYAxisRotation(int16_t value)
{
	_yAxisRotation = value;
	if (_autoSendState) sendState();
}
void Joystick_::setZAxisRotation(int16_t value)
{
	_zAxisRotation = value;
	if (_autoSendState) sendState();
}

void Joystick_::setHatSwitch(int8_t hatSwitchIndex, int16_t value)
{
	_hatSwitch[hatSwitchIndex % 2] = value;
	if (_autoSendState) sendState();
}

void Joystick_::sendState()
{
	uint8_t data[_hidReportSize];

	// Load Button State
	for (int index = 0; index < _hidReportSize; index++)
	{
		data[index] = _buttonValues[index];		
	}

//	data[4] = _throttle;
//	data[5] = _rudder;

	// Calculate hat-switch values
    /*
	uint8_t convertedHatSwitch[2];
	for (int hatSwitchIndex = 0; hatSwitchIndex < 2; hatSwitchIndex++)
	{
		if (_hatSwitch[hatSwitchIndex] < 0)
		{
			convertedHatSwitch[hatSwitchIndex] = 8;
		}
		else
		{
			convertedHatSwitch[hatSwitchIndex] = (_hatSwitch[hatSwitchIndex] % 360) / 45;
		}
	}
    */

	// Pack hat-switch states into a single byte
	//data[6] = (convertedHatSwitch[1] << 4) | (B00001111 & convertedHatSwitch[0]);

	//data[7] = _xAxis + 127;
	//data[8] = _yAxis + 127;
	//data[9] = _zAxis + 127;

	//data[10] = (_xAxisRotation % 360) * 0.708;
	//data[11] = (_yAxisRotation % 360) * 0.708;
	//data[12] = (_zAxisRotation % 360) * 0.708;

	// HID().SendReport(Report number, array of values in same order as HID descriptor, length)
	DynamicHID().SendReport(_hidReportId, data, _hidReportSize);
}

#endif