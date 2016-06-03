/*
  Joystick.cpp

  Copyright (c) 2015-2016, Matthew Heironimus

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

#define JOYSTICK_REPORT_ID_INDEX 7
#define JOYSTICK_AXIS_MINIMUM -32767
#define JOYSTICK_AXIS_MAXIMUM 32767
#define JOYSTICK_SIMULATOR_MINIMUM -32767
#define JOYSTICK_SIMULATOR_MAXIMUM 32767

Joystick_::Joystick_(
	uint8_t hidReportId,
    uint8_t buttonCount,
	uint8_t hatSwitchCount,
	bool includeXAxis,
	bool includeYAxis,
	bool includeZAxis,
	bool includeRxAxis,
	bool includeRyAxis,
	bool includeRzAxis,
	bool includeRudder,
	bool includeThrottle,
	bool includeAccelerator,
	bool includeBrake,
	bool includeSteering)
{
    // Set the USB HID Report ID
    _hidReportId = hidReportId;

    // Save Joystick Settings
    _buttonCount = buttonCount;
	_hatSwitchCount = hatSwitchCount;
	_includeXAxis = includeXAxis;
	_includeYAxis = includeYAxis;
	_includeZAxis = includeZAxis;
	_includeRxAxis = includeRxAxis;
	_includeRyAxis = includeRyAxis;
	_includeRzAxis = includeRzAxis;
	_includeRudder = includeRudder;
	_includeThrottle = includeThrottle;
	_includeAccelerator = includeAccelerator;
	_includeBrake = includeBrake;
	_includeSteering = includeSteering;

    // Build Joystick HID Report Description
	
	// Button Calculations
	uint8_t buttonsInLastByte = _buttonCount % 8;
	uint8_t buttonPaddingBits = 0;
	if (buttonsInLastByte > 0)
	{
		buttonPaddingBits = 8 - buttonsInLastByte;
	}
	
	// Axis Calculations
	uint8_t axisCount = (_includeXAxis == true)
		+  (_includeYAxis == true)
		+  (_includeZAxis == true)
		+  (_includeRxAxis == true)
		+  (_includeRyAxis == true)
		+  (_includeRzAxis == true);
		
	uint8_t simulationCount = (includeRudder == true)
		+ (includeThrottle == true)
		+ (includeAccelerator == true)
		+ (includeBrake == true)
		+ (includeSteering == true); 
		
    uint8_t tempHidReportDescriptor[150];
    int hidReportDescriptorSize = 0;

    // USAGE_PAGE (Generic Desktop)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x05;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // USAGE (Joystick)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x04;

    // COLLECTION (Application)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0xa1;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // REPORT_ID (Default: 3)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x85;
    tempHidReportDescriptor[hidReportDescriptorSize++] = _hidReportId;
	
	if (_buttonCount > 0) {

		// USAGE_PAGE (Button)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x05;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;

		// USAGE_MINIMUM (Button 1)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x19;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

		// USAGE_MAXIMUM (Button 32)            
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x29;
		tempHidReportDescriptor[hidReportDescriptorSize++] = _buttonCount;

		// LOGICAL_MINIMUM (0)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x15;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

		// LOGICAL_MAXIMUM (1)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x25;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

		// REPORT_SIZE (1)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

		// REPORT_COUNT (# of buttons)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
		tempHidReportDescriptor[hidReportDescriptorSize++] = _buttonCount;

		// UNIT_EXPONENT (0)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x55;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

		// UNIT (None)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x65;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

		// INPUT (Data,Var,Abs)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;

		if (buttonPaddingBits > 0) {
			
			// REPORT_SIZE (1)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

			// REPORT_COUNT (# of padding bits)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
			tempHidReportDescriptor[hidReportDescriptorSize++] = buttonPaddingBits;
					
			// INPUT (Const,Var,Abs)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x03;
			
		} // Padding Bits Needed

	} // Buttons

	if ((axisCount > 0) || (_hatSwitchCount > 0)) {
	
		// USAGE_PAGE (Generic Desktop)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x05;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;
		
	}

	if (_hatSwitchCount > 0) {

		// USAGE (Hat Switch)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x39;

		// LOGICAL_MINIMUM (0)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x15;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

		// LOGICAL_MAXIMUM (7)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x25;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x07;

		// PHYSICAL_MINIMUM (0)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x35;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

		// PHYSICAL_MAXIMUM (315)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x46;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x3B;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

		// UNIT (Eng Rot:Angular Pos)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x65;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x14;

		// REPORT_SIZE (4)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x04;

		// REPORT_COUNT (1)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;
						
		// INPUT (Data,Var,Abs)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;
		
		if (_hatSwitchCount > 1) {
			
			// USAGE (Hat Switch)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x39;

			// LOGICAL_MINIMUM (0)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x15;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

			// LOGICAL_MAXIMUM (7)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x25;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x07;

			// PHYSICAL_MINIMUM (0)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x35;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

			// PHYSICAL_MAXIMUM (315)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x46;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x3B;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

			// UNIT (Eng Rot:Angular Pos)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x65;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x14;

			// REPORT_SIZE (4)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x04;

			// REPORT_COUNT (1)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;
							
			// INPUT (Data,Var,Abs)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;
		
		} else {
		
			// Use Padding Bits
		
			// REPORT_SIZE (1)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

			// REPORT_COUNT (4)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x04;
					
			// INPUT (Const,Var,Abs)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x03;
			
		} // One or Two Hat Switches?

	} // Hat Switches

	if (axisCount > 0) {
	
		// USAGE (Pointer)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

		// LOGICAL_MINIMUM (-32767)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x16;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x80;

		// LOGICAL_MAXIMUM (+32767)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x26;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0xFF;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x7F;

		// REPORT_SIZE (16)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x10;

		// REPORT_COUNT (axisCount)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
		tempHidReportDescriptor[hidReportDescriptorSize++] = axisCount;
						
		// COLLECTION (Physical)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0xA1;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

		if (_includeXAxis == true) {
			// USAGE (X)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x30;
		}

		if (_includeYAxis == true) {
			// USAGE (Y)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x31;
		}
		
		if (_includeZAxis == true) {
			// USAGE (Z)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x32;
		}
		
		if (_includeRxAxis == true) {
			// USAGE (Rx)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x33;
		}
		
		if (_includeRyAxis == true) {
			// USAGE (Ry)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x34;
		}
		
		if (_includeRzAxis == true) {
			// USAGE (Rz)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x35;
		}
		
		// INPUT (Data,Var,Abs)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;
		
		// END_COLLECTION (Physical)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0xc0;
		
	} // X, Y, Z, Rx, Ry, and Rz Axis	
	
	if (simulationCount > 0) {
	
		// USAGE_PAGE (Simulation Controls)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x05;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;
		
		// LOGICAL_MINIMUM (-32767)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x16;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x80;

		// LOGICAL_MAXIMUM (+32767)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x26;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0xFF;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x7F;

		// REPORT_SIZE (16)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x10;

		// REPORT_COUNT (simulationCount)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
		tempHidReportDescriptor[hidReportDescriptorSize++] = simulationCount;

		// COLLECTION (Physical)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0xA1;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

		if (_includeRudder == true) {
			// USAGE (Rudder)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0xBA;
		}

		if (_includeThrottle == true) {
			// USAGE (Throttle)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0xBB;
		}

		if (_includeAccelerator == true) {
			// USAGE (Accelerator)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0xC4;
		}

		if (_includeBrake == true) {
			// USAGE (Brake)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0xC5;
		}

		if (_includeSteering == true) {
			// USAGE (Steering)
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
			tempHidReportDescriptor[hidReportDescriptorSize++] = 0xC8;
		}

		// INPUT (Data,Var,Abs)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;
		
		// END_COLLECTION (Physical)
		tempHidReportDescriptor[hidReportDescriptorSize++] = 0xc0;
	
	} // Simulation Controls

    // END_COLLECTION
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0xc0;

	// Create a copy of the HID Report Descriptor template that is just the right size
	_customHidReportDescriptor = new uint8_t[hidReportDescriptorSize];
	memcpy(_customHidReportDescriptor, tempHidReportDescriptor, hidReportDescriptorSize);
	
	// Register HID Report Description
	_node = new DynamicHIDSubDescriptor(_customHidReportDescriptor, hidReportDescriptorSize, false);
	DynamicHID().AppendDescriptor(_node);
	
    // Setup Joystick State
	if (buttonCount > 0) {
		_buttonValuesArraySize = _buttonCount / 8;
		if ((_buttonCount % 8) > 0) {
			_buttonValuesArraySize++;
		}
		_buttonValues = new uint8_t[_buttonValuesArraySize];
	}
	
	// Calculate HID Report Size
	_hidReportSize = _buttonValuesArraySize;
	_hidReportSize += (_hatSwitchCount > 0);
	_hidReportSize += (axisCount * 2);
	_hidReportSize += (simulationCount * 2);
	
	// Initalize Joystick State
	_xAxis = 0;
	_yAxis = 0;
	_zAxis = 0;
	_xAxisRotation = 0;
	_yAxisRotation = 0;
	_zAxisRotation = 0;
	_throttle = 0;
	_rudder = 0;
	_accelerator = 0;
	_brake = 0;
	_steering = 0;
	for (int index = 0; index < JOYSTICK_HATSWITCH_COUNT_MAXIMUM; index++)
	{
		_hatSwitchValues[index] = -1;
	}
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

void Joystick_::setRudderRange(int16_t minimum, int16_t maximum)
{
	_rudderMinimum = minimum;
	_rudderMaximum = maximum;
}

void Joystick_::setThrottleRange(int16_t minimum, int16_t maximum)
{
	_throttleMinimum = minimum;
	_throttleMaximum = maximum;
}

void Joystick_::setAcceleratorRange(int16_t minimum, int16_t maximum)
{
	_acceleratorMinimum = minimum;
	_acceleratorMaximum = maximum;
}

void Joystick_::setBrakeRange(int16_t minimum, int16_t maximum)
{
	_brakeMinimum = minimum;
	_brakeMaximum = maximum;
}

void Joystick_::setSteeringRange(int16_t minimum, int16_t maximum)
{
	_steeringMinimum = minimum;
	_steeringMaximum = maximum;
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

void Joystick_::setRudder(int16_t value)
{
	if ((value < _rudderMinimum) || (value > _rudderMaximum)) return;
	
	_rudder = value;
	if (_autoSendState) sendState();
}
void Joystick_::setThrottle(int16_t value)
{
	if ((value < _throttleMinimum) || (value > _throttleMaximum)) return;
	
	_throttle = value;
	if (_autoSendState) sendState();
}
void Joystick_::setAccelerator(int16_t value)
{
	if ((value < _acceleratorMinimum) || (value > _acceleratorMaximum)) return;

	_accelerator = value;
	if (_autoSendState) sendState();
}
void Joystick_::setBrake(int16_t value)
{
	if ((value < _brakeMinimum) || (value > _brakeMaximum)) return;

	_brake = value;
	if (_autoSendState) sendState();
}
void Joystick_::setSteering(int16_t value)
{
	if ((value < _steeringMinimum) || (value > _steeringMaximum)) return;

	_steering = value;
	if (_autoSendState) sendState();
}

void Joystick_::setHatSwitch(int8_t hatSwitchIndex, int16_t value)
{
	if (hatSwitchIndex >= _hatSwitchCount) return;
	
	_hatSwitchValues[hatSwitchIndex] = value;
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

	// Set Hat Switch Values
	if (_hatSwitchCount > 0) {
		
		// Calculate hat-switch values
		uint8_t convertedHatSwitch[JOYSTICK_HATSWITCH_COUNT_MAXIMUM];
		for (int hatSwitchIndex = 0; hatSwitchIndex < JOYSTICK_HATSWITCH_COUNT_MAXIMUM; hatSwitchIndex++)
		{
			if (_hatSwitchValues[hatSwitchIndex] < 0)
			{
				convertedHatSwitch[hatSwitchIndex] = 8;
			}
			else
			{
				convertedHatSwitch[hatSwitchIndex] = (_hatSwitchValues[hatSwitchIndex] % 360) / 45;
			}			
		}

		// Pack hat-switch states into a single byte
		data[index++] = (convertedHatSwitch[1] << 4) | (B00001111 & convertedHatSwitch[0]);
	
	} // Hat Switches

	// Set Axis Values
	int16_t convertedValue;
	uint8_t highByte;
	uint8_t lowByte;
	if (_includeXAxis == true) {
		convertedValue = map(_xAxis, _xAxisMinimum, _xAxisMaximum, JOYSTICK_AXIS_MINIMUM, JOYSTICK_AXIS_MAXIMUM);
				
		highByte = (uint8_t)(convertedValue >> 8);
		lowByte = (uint8_t)(convertedValue & 0x00FF);
		
		data[index++] = lowByte;
		data[index++] = highByte;
	}
	if (_includeYAxis == true) {
		convertedValue = map(_yAxis, _yAxisMinimum, _yAxisMaximum, JOYSTICK_AXIS_MINIMUM, JOYSTICK_AXIS_MAXIMUM);
				
		highByte = (uint8_t)(convertedValue >> 8);
		lowByte = (uint8_t)(convertedValue & 0x00FF);
		
		data[index++] = lowByte;
		data[index++] = highByte;
	}
	if (_includeZAxis == true) {
		convertedValue = map(_zAxis, _zAxisMinimum, _zAxisMaximum, JOYSTICK_AXIS_MINIMUM, JOYSTICK_AXIS_MAXIMUM);
				
		highByte = (uint8_t)(convertedValue >> 8);
		lowByte = (uint8_t)(convertedValue & 0x00FF);
		
		data[index++] = lowByte;
		data[index++] = highByte;
	}
	if (_includeRxAxis == true) {
		convertedValue = map(_xAxisRotation, _rxAxisMinimum, _rxAxisMaximum, JOYSTICK_AXIS_MINIMUM, JOYSTICK_AXIS_MAXIMUM);
				
		highByte = (uint8_t)(convertedValue >> 8);
		lowByte = (uint8_t)(convertedValue & 0x00FF);
		
		data[index++] = lowByte;
		data[index++] = highByte;
	}
	if (_includeRyAxis == true) {
		convertedValue = map(_yAxisRotation, _ryAxisMinimum, _ryAxisMaximum, JOYSTICK_AXIS_MINIMUM, JOYSTICK_AXIS_MAXIMUM);
				
		highByte = (uint8_t)(convertedValue >> 8);
		lowByte = (uint8_t)(convertedValue & 0x00FF);
		
		data[index++] = lowByte;
		data[index++] = highByte;
	}
	if (_includeRzAxis == true) {
		convertedValue = map(_zAxisRotation, _rzAxisMinimum, _rzAxisMaximum, JOYSTICK_AXIS_MINIMUM, JOYSTICK_AXIS_MAXIMUM);
				
		highByte = (uint8_t)(convertedValue >> 8);
		lowByte = (uint8_t)(convertedValue & 0x00FF);
		
		data[index++] = lowByte;
		data[index++] = highByte;
	}
	if (_includeRudder == true) {
		convertedValue = map(_rudder, _rudderMinimum, _rudderMaximum, JOYSTICK_SIMULATOR_MINIMUM, JOYSTICK_SIMULATOR_MAXIMUM);
				
		highByte = (uint8_t)(convertedValue >> 8);
		lowByte = (uint8_t)(convertedValue & 0x00FF);
		
		data[index++] = lowByte;
		data[index++] = highByte;
	}
	if (_includeThrottle == true) {
		convertedValue = map(_throttle, _throttleMinimum, _throttleMaximum, JOYSTICK_SIMULATOR_MINIMUM, JOYSTICK_SIMULATOR_MAXIMUM);
				
		highByte = (uint8_t)(convertedValue >> 8);
		lowByte = (uint8_t)(convertedValue & 0x00FF);
		
		data[index++] = lowByte;
		data[index++] = highByte;
	}
	if (_includeAccelerator == true) {
		convertedValue = map(_accelerator, _acceleratorMinimum, _acceleratorMaximum, JOYSTICK_SIMULATOR_MINIMUM, JOYSTICK_SIMULATOR_MAXIMUM);
				
		highByte = (uint8_t)(convertedValue >> 8);
		lowByte = (uint8_t)(convertedValue & 0x00FF);
		
		data[index++] = lowByte;
		data[index++] = highByte;
	}
	if (_includeBrake == true) {
		convertedValue = map(_brake, _brakeMinimum, _brakeMaximum, JOYSTICK_SIMULATOR_MINIMUM, JOYSTICK_SIMULATOR_MAXIMUM);
				
		highByte = (uint8_t)(convertedValue >> 8);
		lowByte = (uint8_t)(convertedValue & 0x00FF);
		
		data[index++] = lowByte;
		data[index++] = highByte;
	}
	if (_includeSteering == true) {
		convertedValue = map(_steering, _steeringMinimum, _steeringMaximum, JOYSTICK_SIMULATOR_MINIMUM, JOYSTICK_SIMULATOR_MAXIMUM);
				
		highByte = (uint8_t)(convertedValue >> 8);
		lowByte = (uint8_t)(convertedValue & 0x00FF);
		
		data[index++] = lowByte;
		data[index++] = highByte;
	}

	DynamicHID().SendReport(_hidReportId, data, _hidReportSize);
}

#endif
