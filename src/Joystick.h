/*
  Joystick.h

  Copyright (c) 2015-2017, Matthew Heironimus

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

#include "DynamicHID/DynamicHID.h"

#if ARDUINO < 10606
#error The Joystick library requires Arduino IDE 1.6.6 or greater. Please update your IDE.
#endif // ARDUINO < 10606

#if ARDUINO > 10606
#if !defined(USBCON)
#error The Joystick library can only be used with a USB MCU (e.g. Arduino Leonardo, Arduino Micro, etc.).
#endif // !defined(USBCON)
#endif // ARDUINO > 10606

#if !defined(_USING_DYNAMIC_HID)

#warning "Using legacy HID core (non pluggable)"

#else // !defined(_USING_DYNAMIC_HID)

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
#define JOYSTICK_HATSWITCH_RELEASE           -1
#define JOYSTICK_TYPE_JOYSTICK             0x04
#define JOYSTICK_TYPE_GAMEPAD              0x05
#define JOYSTICK_TYPE_MULTI_AXIS           0x08

class Joystick_
{
private:

    // Joystick State
    int32_t   _xAxis;
    int32_t   _yAxis;
    int32_t   _zAxis;
    int32_t   _xAxisRotation;
    int32_t   _yAxisRotation;
    int32_t   _zAxisRotation;
    int32_t   _throttle;
    int32_t   _rudder;
    int32_t   _accelerator;
    int32_t   _brake;
    int32_t   _steering;
    int16_t    _hatSwitchValues[JOYSTICK_HATSWITCH_COUNT_MAXIMUM];
    uint8_t   *_buttonValues = NULL;

    // Joystick Settings
    bool     _autoSendState;
    uint8_t  _buttonCount;
    uint8_t  _buttonValuesArraySize = 0;
    uint8_t  _hatSwitchCount;
    uint8_t  _includeAxisFlags;
    uint8_t  _includeSimulatorFlags;
    int32_t  _xAxisMinimum = JOYSTICK_DEFAULT_AXIS_MINIMUM;
    int32_t  _xAxisMaximum = JOYSTICK_DEFAULT_AXIS_MAXIMUM;
    int32_t  _yAxisMinimum = JOYSTICK_DEFAULT_AXIS_MINIMUM;
    int32_t  _yAxisMaximum = JOYSTICK_DEFAULT_AXIS_MAXIMUM;
    int32_t  _zAxisMinimum = JOYSTICK_DEFAULT_AXIS_MINIMUM;
    int32_t  _zAxisMaximum = JOYSTICK_DEFAULT_AXIS_MAXIMUM;
    int32_t  _rxAxisMinimum = JOYSTICK_DEFAULT_AXIS_MINIMUM;
    int32_t  _rxAxisMaximum = JOYSTICK_DEFAULT_AXIS_MAXIMUM;
    int32_t  _ryAxisMinimum = JOYSTICK_DEFAULT_AXIS_MINIMUM;
    int32_t  _ryAxisMaximum = JOYSTICK_DEFAULT_AXIS_MAXIMUM;
    int32_t  _rzAxisMinimum = JOYSTICK_DEFAULT_AXIS_MINIMUM;
    int32_t  _rzAxisMaximum = JOYSTICK_DEFAULT_AXIS_MAXIMUM;
    int32_t  _rudderMinimum = JOYSTICK_DEFAULT_SIMULATOR_MINIMUM;
    int32_t  _rudderMaximum = JOYSTICK_DEFAULT_SIMULATOR_MAXIMUM;
    int32_t  _throttleMinimum = JOYSTICK_DEFAULT_SIMULATOR_MINIMUM;
    int32_t  _throttleMaximum = JOYSTICK_DEFAULT_SIMULATOR_MAXIMUM;
    int32_t  _acceleratorMinimum = JOYSTICK_DEFAULT_SIMULATOR_MINIMUM;
    int32_t  _acceleratorMaximum = JOYSTICK_DEFAULT_SIMULATOR_MAXIMUM;
    int32_t  _brakeMinimum = JOYSTICK_DEFAULT_SIMULATOR_MINIMUM;
    int32_t  _brakeMaximum = JOYSTICK_DEFAULT_SIMULATOR_MAXIMUM;
    int32_t  _steeringMinimum = JOYSTICK_DEFAULT_SIMULATOR_MINIMUM;
    int32_t  _steeringMaximum = JOYSTICK_DEFAULT_SIMULATOR_MAXIMUM;

    uint8_t   _hidReportId;
    uint8_t   _hidReportSize; 

protected:
    int buildAndSet16BitValue(bool includeValue, int32_t value, int32_t valueMinimum, int32_t valueMaximum, int32_t actualMinimum, int32_t actualMaximum, uint8_t dataLocation[]);
    int buildAndSetAxisValue(bool includeAxis, int32_t axisValue, int32_t axisMinimum, int32_t axisMaximum, uint8_t dataLocation[]);
    int buildAndSetSimulationValue(bool includeValue, int32_t value, int32_t valueMinimum, int32_t valueMaximum, uint8_t dataLocation[]);

public:
    Joystick_(
        uint8_t hidReportId = JOYSTICK_DEFAULT_REPORT_ID,
        uint8_t joystickType = JOYSTICK_TYPE_JOYSTICK,
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
    
    // Set Range Functions
    inline void setXAxisRange(int32_t minimum, int32_t maximum)
    {
        _xAxisMinimum = minimum;
        _xAxisMaximum = maximum;
    }
    inline void setYAxisRange(int32_t minimum, int32_t maximum)
    {
        _yAxisMinimum = minimum;
        _yAxisMaximum = maximum;
    }
    inline void setZAxisRange(int32_t minimum, int32_t maximum)
    {
        _zAxisMinimum = minimum;
        _zAxisMaximum = maximum;
    }
    inline void setRxAxisRange(int32_t minimum, int32_t maximum)
    {
        _rxAxisMinimum = minimum;
        _rxAxisMaximum = maximum;
    }
    inline void setRyAxisRange(int32_t minimum, int32_t maximum)
    {
        _ryAxisMinimum = minimum;
        _ryAxisMaximum = maximum;
    }
    inline void setRzAxisRange(int32_t minimum, int32_t maximum)
    {
        _rzAxisMinimum = minimum;
        _rzAxisMaximum = maximum;
    }
    inline void setRudderRange(int32_t minimum, int32_t maximum)
    {
        _rudderMinimum = minimum;
        _rudderMaximum = maximum;
    }
    inline void setThrottleRange(int32_t minimum, int32_t maximum)
    {
        _throttleMinimum = minimum;
        _throttleMaximum = maximum;
    }
    inline void setAcceleratorRange(int32_t minimum, int32_t maximum)
    {
        _acceleratorMinimum = minimum;
        _acceleratorMaximum = maximum;
    }
    inline void setBrakeRange(int32_t minimum, int32_t maximum)
    {
        _brakeMinimum = minimum;
        _brakeMaximum = maximum;
    }
    inline void setSteeringRange(int32_t minimum, int32_t maximum)
    {
        _steeringMinimum = minimum;
        _steeringMaximum = maximum;
    }

    // Set Axis Values
    void setXAxis(int32_t value);
    void setYAxis(int32_t value);
    void setZAxis(int32_t value);
    void setRxAxis(int32_t value);
    void setRyAxis(int32_t value);
    void setRzAxis(int32_t value);

    // Set Simulation Values
    void setRudder(int32_t value);
    void setThrottle(int32_t value);
    void setAccelerator(int32_t value);
    void setBrake(int32_t value);
    void setSteering(int32_t value);

    void setButton(uint8_t button, uint8_t value);
    void pressButton(uint8_t button);
    void releaseButton(uint8_t button);

    void setHatSwitch(int8_t hatSwitch, int16_t value);

    void sendState();
};

#endif // !defined(_USING_DYNAMIC_HID)
#endif // JOYSTICK_h
