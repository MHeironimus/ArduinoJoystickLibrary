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
#define JOYSTICK_AXIS_MINIMUM -32767
#define JOYSTICK_AXIS_MAXIMUM 32767

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
    uint8_t buttonCount,
	bool includeXAxis,
	bool includeYAxis,
	bool includeZAxis,
	bool includeRxAxis,
	bool includeRyAxis,
	bool includeRzAxis)
{
    // Set the USB HID Report ID
    _hidReportId = hidReportId;

    // Save Joystick Settings
    _buttonCount = buttonCount;
	_includeXAxis = includeXAxis;
	_includeYAxis = includeYAxis;
	_includeZAxis = includeZAxis;
	_includeRxAxis = includeRxAxis;
	_includeRyAxis = includeRyAxis;
	_includeRzAxis = includeRzAxis;

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
	
	if (_buttonCount > 0) {

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
			
		} // Padding Bits Needed

	} // Buttons
	
	// USAGE_PAGE (Generic Desktop)
	customHidReportDescriptor[hidReportDescriptorSize++] = 0x05;
	customHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // USAGE (Pointer)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

	// LOGICAL_MINIMUM (-32767)
	customHidReportDescriptor[hidReportDescriptorSize++] = 0x16;
	customHidReportDescriptor[hidReportDescriptorSize++] = 0x01;
	customHidReportDescriptor[hidReportDescriptorSize++] = 0x80;

	// LOGICAL_MAXIMUM (+32767)
	customHidReportDescriptor[hidReportDescriptorSize++] = 0x26;
	customHidReportDescriptor[hidReportDescriptorSize++] = 0xFF;
	customHidReportDescriptor[hidReportDescriptorSize++] = 0x7F;

	// REPORT_SIZE (16)
	customHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
	customHidReportDescriptor[hidReportDescriptorSize++] = 0x10;

	// REPORT_COUNT (6)
	customHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
	customHidReportDescriptor[hidReportDescriptorSize++] = 0x06;
					
    // COLLECTION (Physical)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0xA1;
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

    // USAGE (X)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x30;

    // USAGE (Y)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x31;

    // USAGE (Z)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x32;

    // USAGE (Rx)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x33;

    // USAGE (Ry)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x34;

    // USAGE (Rz)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    customHidReportDescriptor[hidReportDescriptorSize++] = 0x35;

	// INPUT (Data,Var,Abs)
	customHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
	customHidReportDescriptor[hidReportDescriptorSize++] = 0x02;
	
    // END_COLLECTION (Physical)
    customHidReportDescriptor[hidReportDescriptorSize++] = 0xc0;
	
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
	_hidReportSize += (6 * 2);

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

void Joystick_::setXAxisRange(int16_t minimum, int16_t maximum)
{
	_xAxisMinimum = minimum;
	_xAxisMaximum = maximum;
}

void Joystick_::setYAxisRange(int16_t minimum, int16_t maximum)
{
	_yAxisMinimum = minimum;
	_yAxisMaximum = maximum;
}

void Joystick_::setZAxisRange(int16_t minimum, int16_t maximum)
{
	_zAxisMinimum = minimum;
	_zAxisMaximum = maximum;
}

void Joystick_::setRxAxisRange(int16_t minimum, int16_t maximum)
{
	_rxAxisMinimum = minimum;
	_rxAxisMaximum = maximum;
}

void Joystick_::setRyAxisRange(int16_t minimum, int16_t maximum)
{
	_ryAxisMinimum = minimum;
	_ryAxisMaximum = maximum;
}

void Joystick_::setRzAxisRange(int16_t minimum, int16_t maximum)
{
	_rzAxisMinimum = minimum;
	_rzAxisMaximum = maximum;
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
	if ((value < _xAxisMinimum) || (value > _xAxisMaximum)) return;
	
	_xAxis = value;
	if (_autoSendState) sendState();
}
void Joystick_::setYAxis(int16_t value)
{
	if ((value < _yAxisMinimum) || (value > _yAxisMaximum)) return;
	
	_yAxis = value;
	if (_autoSendState) sendState();
}
void Joystick_::setZAxis(int16_t value)
{
	if ((value < _zAxisMinimum) || (value > _zAxisMaximum)) return;

	_zAxis = value;
	if (_autoSendState) sendState();
}

void Joystick_::setRxAxis(int16_t value)
{
	if ((value < _rxAxisMinimum) || (value > _rxAxisMaximum)) return;

	_xAxisRotation = value;
	if (_autoSendState) sendState();
}
void Joystick_::setRyAxis(int16_t value)
{
	if ((value < _ryAxisMinimum) || (value > _ryAxisMaximum)) return;

	_yAxisRotation = value;
	if (_autoSendState) sendState();
}
void Joystick_::setRzAxis(int16_t value)
{
	if ((value < _rzAxisMinimum) || (value > _rzAxisMaximum)) return;

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
	int index = 0;
	
	// Load Button State
	for (; index < _buttonValuesArraySize; index++)
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

	// Set Axis Values
	int16_t axisValue;
	uint8_t highByte;
	uint8_t lowByte;
	if (_includeXAxis == true) {
		axisValue = map(_xAxis, _xAxisMinimum, _xAxisMaximum, JOYSTICK_AXIS_MINIMUM, JOYSTICK_AXIS_MAXIMUM);
				
		highByte = (uint8_t)(axisValue >> 8);
		lowByte = (uint8_t)(axisValue & 0x00FF);
		
		data[index++] = lowByte;
		data[index++] = highByte;
	}
	if (_includeYAxis == true) {
		axisValue = map(_yAxis, _yAxisMinimum, _yAxisMaximum, JOYSTICK_AXIS_MINIMUM, JOYSTICK_AXIS_MAXIMUM);
				
		highByte = (uint8_t)(axisValue >> 8);
		lowByte = (uint8_t)(axisValue & 0x00FF);
		
		data[index++] = lowByte;
		data[index++] = highByte;
	}
	if (_includeZAxis == true) {
		axisValue = map(_zAxis, _zAxisMinimum, _zAxisMaximum, JOYSTICK_AXIS_MINIMUM, JOYSTICK_AXIS_MAXIMUM);
				
		highByte = (uint8_t)(axisValue >> 8);
		lowByte = (uint8_t)(axisValue & 0x00FF);
		
		data[index++] = lowByte;
		data[index++] = highByte;
	}
	if (_includeRxAxis == true) {
		axisValue = map(_xAxisRotation, _rxAxisMinimum, _rxAxisMaximum, JOYSTICK_AXIS_MINIMUM, JOYSTICK_AXIS_MAXIMUM);
				
		highByte = (uint8_t)(axisValue >> 8);
		lowByte = (uint8_t)(axisValue & 0x00FF);
		
		data[index++] = lowByte;
		data[index++] = highByte;
	}
	if (_includeRyAxis == true) {
		axisValue = map(_yAxisRotation, _ryAxisMinimum, _ryAxisMaximum, JOYSTICK_AXIS_MINIMUM, JOYSTICK_AXIS_MAXIMUM);
				
		highByte = (uint8_t)(axisValue >> 8);
		lowByte = (uint8_t)(axisValue & 0x00FF);
		
		data[index++] = lowByte;
		data[index++] = highByte;
	}
	if (_includeRzAxis == true) {
		axisValue = map(_zAxisRotation, _rzAxisMinimum, _rzAxisMaximum, JOYSTICK_AXIS_MINIMUM, JOYSTICK_AXIS_MAXIMUM);
				
		highByte = (uint8_t)(axisValue >> 8);
		lowByte = (uint8_t)(axisValue & 0x00FF);
		
		data[index++] = lowByte;
		data[index++] = highByte;
	}

/*
	Serial.print("About to send report. _hidReportSize = ");
	Serial.print(_hidReportSize);
	Serial.print("; index = ");
	Serial.print(index);
	Serial.print("; xAxis = ");
	Serial.print(_xAxis);
	Serial.print("; _zAxisRotation = ");
	Serial.print(_zAxisRotation);
	Serial.print("; _zAxisRotation Mapped Value = ");
	Serial.println(axisValue);
*/

	DynamicHID().SendReport(_hidReportId, data, _hidReportSize);
}

#endif