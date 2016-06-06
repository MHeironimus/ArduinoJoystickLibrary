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

#include <DynamicHID.h>

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
//  Joystick (Gamepad)

#define JOYSTICK_DEFAULT_REPORT_ID         0x03
#define JOYSTICK_DEFAULT_BUTTON_COUNT        32
#define JOYSTICK_DEFAULT_AXIS_MINIMUM         0
#define JOYSTICK_DEFAULT_AXIS_MAXIMUM      1023
#define JOYSTICK_DEFAULT_SIMULATOR_MINIMUM    0
#define JOYSTICK_DEFAULT_SIMULATOR_MAXIMUM 1023
#define JOYSTICK_DEFAULT_HATSWITCH_COUNT      2
#define JOYSTICK_HATSWITCH_COUNT_MAXIMUM      2

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
	int16_t					 _accelerator;
	int16_t					 _brake;
	int16_t					 _steering;
	int16_t	                 _hatSwitchValues[JOYSTICK_HATSWITCH_COUNT_MAXIMUM];
    uint8_t                 *_buttonValues = NULL;

    // Joystick Settings
    bool                     _autoSendState;
    uint8_t                  _buttonCount;
    int                      _buttonValuesArraySize = 0;
	uint8_t					 _hatSwitchCount;
	bool                     _includeXAxis;
	bool                     _includeYAxis;
	bool                     _includeZAxis;
	bool                     _includeRxAxis;
	bool                     _includeRyAxis;
	bool                     _includeRzAxis;
	bool                     _includeRudder;
	bool                     _includeThrottle;
	bool                     _includeAccelerator;
	bool                     _includeBrake;
	bool                     _includeSteering;
	int16_t                  _xAxisMinimum = JOYSTICK_DEFAULT_AXIS_MINIMUM;
	int16_t                  _xAxisMaximum = JOYSTICK_DEFAULT_AXIS_MAXIMUM;
	int16_t                  _yAxisMinimum = JOYSTICK_DEFAULT_AXIS_MINIMUM;
	int16_t                  _yAxisMaximum = JOYSTICK_DEFAULT_AXIS_MAXIMUM;
	int16_t                  _zAxisMinimum = JOYSTICK_DEFAULT_AXIS_MINIMUM;
	int16_t                  _zAxisMaximum = JOYSTICK_DEFAULT_AXIS_MAXIMUM;
	int16_t                  _rxAxisMinimum = JOYSTICK_DEFAULT_AXIS_MINIMUM;
	int16_t                  _rxAxisMaximum = JOYSTICK_DEFAULT_AXIS_MAXIMUM;
	int16_t                  _ryAxisMinimum = JOYSTICK_DEFAULT_AXIS_MINIMUM;
	int16_t                  _ryAxisMaximum = JOYSTICK_DEFAULT_AXIS_MAXIMUM;
	int16_t                  _rzAxisMinimum = JOYSTICK_DEFAULT_AXIS_MINIMUM;
	int16_t                  _rzAxisMaximum = JOYSTICK_DEFAULT_AXIS_MAXIMUM;
	int16_t                  _rudderMinimum = JOYSTICK_DEFAULT_SIMULATOR_MINIMUM;
	int16_t                  _rudderMaximum = JOYSTICK_DEFAULT_SIMULATOR_MAXIMUM;
	int16_t                  _throttleMinimum = JOYSTICK_DEFAULT_SIMULATOR_MINIMUM;
	int16_t                  _throttleMaximum = JOYSTICK_DEFAULT_SIMULATOR_MAXIMUM;
	int16_t                  _acceleratorMinimum = JOYSTICK_DEFAULT_SIMULATOR_MINIMUM;
	int16_t                  _acceleratorMaximum = JOYSTICK_DEFAULT_SIMULATOR_MAXIMUM;
	int16_t                  _brakeMinimum = JOYSTICK_DEFAULT_SIMULATOR_MINIMUM;
	int16_t                  _brakeMaximum = JOYSTICK_DEFAULT_SIMULATOR_MAXIMUM;
	int16_t                  _steeringMinimum = JOYSTICK_DEFAULT_SIMULATOR_MINIMUM;
	int16_t                  _steeringMaximum = JOYSTICK_DEFAULT_SIMULATOR_MAXIMUM;

	uint8_t					*_customHidReportDescriptor = NULL;
	DynamicHIDSubDescriptor *_node = NULL;
	uint8_t                  _hidReportId;
	int                      _hidReportSize; 

protected:
	int buildAndSet16BitValue(bool includeValue, int16_t value, int16_t valueMinimum, int16_t valueMaximum, int16_t actualMinimum, int16_t actualMaximum, uint8_t dataLocation[]);
	int buildAndSetAxisValue(bool includeAxis, int16_t axisValue, int16_t axisMinimum, int16_t axisMaximum, uint8_t dataLocation[]);
	int buildAndSetSimulationValue(bool includeValue, int16_t value, int16_t valueMinimum, int16_t valueMaximum, uint8_t dataLocation[]);

public:
	Joystick_(
		uint8_t hidReportId = JOYSTICK_DEFAULT_REPORT_ID,
        uint8_t buttonCount = JOYSTICK_DEFAULT_BUTTON_COUNT,
		uint8_t hatSwitchCount = JOYSTICK_DEFAULT_HATSWITCH_COUNT,
		bool includeXAxis = true,
		bool includeYAxis = true,
		bool includeZAxis = true,
		bool includeRxAxis = true,
		bool includeRyAxis = true,
		bool includeRzAxis = true,
		bool includeRudder = true,
		bool includeThrottle = true,
		bool includeAccelerator = true,
		bool includeBrake = true,
		bool includeSteering = true);

	void begin(bool initAutoSendState = true);
	void end();
	
	void setXAxisRange(int16_t minimum, int16_t maximum);
	void setYAxisRange(int16_t minimum, int16_t maximum);
	void setZAxisRange(int16_t minimum, int16_t maximum);
	void setRxAxisRange(int16_t minimum, int16_t maximum);
	void setRyAxisRange(int16_t minimum, int16_t maximum);
	void setRzAxisRange(int16_t minimum, int16_t maximum);

	void setXAxis(int16_t value);
	void setYAxis(int16_t value);
	void setZAxis(int16_t value);
	void setRxAxis(int16_t value);
	void setRyAxis(int16_t value);
	void setRzAxis(int16_t value);

	void setRudderRange(int16_t minimum, int16_t maximum);
	void setThrottleRange(int16_t minimum, int16_t maximum);
	void setAcceleratorRange(int16_t minimum, int16_t maximum);
	void setBrakeRange(int16_t minimum, int16_t maximum);
	void setSteeringRange(int16_t minimum, int16_t maximum);

	void setRudder(int16_t value);
	void setThrottle(int16_t value);
	void setAccelerator(int16_t value);
	void setBrake(int16_t value);
	void setSteering(int16_t value);

	void setButton(uint8_t button, uint8_t value);
	void pressButton(uint8_t button);
	void releaseButton(uint8_t button);

	void setHatSwitch(int8_t hatSwitch, int16_t value);

	void sendState();
};

#endif
#endif
