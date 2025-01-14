#include "Arduino.h"
#define Thermistor false      //Do not change!
#define Thermocouple true  //Do not change!
#define PWM 0                  //Do not change!
#define DAC 1                   //Do not change!
#define Digital 2                 //Do not change!
#define Constant true         //Do not change!
#define Profile false           //Do not change!
#define MATLAB true
#define SerialMonitor false

//-----WARNING!!! Set the parameters below correctly before flashing this code------------------------------
//Either "Constant" setpoint or a present ramp "Profile" can be used for setpoint
#define setpointType Constant
//Either "DAC" or "PWM" or "Digital" can be used for controlling the SSR.
#define outputType Digital
//Set to "true" to use a simple Kalman Filter for the temperature probe input
#define filterInput false      
//Simply change "Thermistor" to "Thermocouple" to swap the type of temperature sensor used 
#define TemperatureProbeType Thermistor

#define debugType MATLAB
//-----------------------------------------------------------------------------------------------------------------------------

//--------------------Nothing to Change here----------------------------------------------------------------------------
#if outputType == 0
#define PWMout
#elif outputType == 1
#define DACout
#else
#define DIGITALout
#endif

#if (TemperatureProbeType)
#define MAX6675
#else
#define ADC
#endif

#if (filterInput)
#define KalmanFilter
#else
#define LinearAverage
#endif

#if (setpointType)
#define ConstantSetpoint
#else
#define ProfiledSetpoint
#endif

#if (debugType)
#define MATLABout
#else
#define SerialMonitorOut
#endif
//-----------------------------------------------------------------------------------------------------------------------------

//----------You can change some pin definations below if required------------------------------------------------
#ifdef DACout
#define DAC1 25                                         //DAC Channel 1 (can't be changed)
#define DAC2 26                                         //DAC Channel 2 (can't be changed)
#define SSR_Pin DAC1
#elif defined(PWMout)
#define PWMPin 25                                     //PWM pin(can be changed to any pin from 0 to 34)
#define SSR_Pin PWMPin
#else
#define digitalPin 25
#define SSR_Pin digitalPin
#endif

#define FanPWMPin 26

#ifdef MAX6675
#define ThermocoupleSO 19                         //MISO
//#define ThermocoupleSI 23                          //MOSI
#define ThermocoupleCS 5
#define ThermocoupleSCK 18
#define sensorReadDelay_ms 300                //Time delay between consecutive reads... min. possible value is 250 ms.
#else
#define ThermistorPin 34
#define adcMax 4095
#define Vs 3.3
//#define sensorReadDelay_ms 50                //Time delay between consecutive reads... min. possible value is 1 ms.
#endif

#define supplyVoltage 3.3                         //Use 3.3 if directly measuring output from one of the DAC pins or set to whatever the supply/source voltage you use (for example, if you use a transitor connected to 5V then set to 5)

#define incPin 15                                     //Pin used to increase the output voltage of the DAC(can be changed to anyother input pin)
#define decPin 4                                      //Pin used to decrease the output voltage of the DAC(can be changed to anyother input pin)
#define updateDelay_ms 100                    //This is simply the delay of the loop function in milliseconds. Increase/decrease to speed up the rate of change of voltage when increase or decrease buttons are pressed.

//-----------------------------------------------------------------------------------------------------------------------------

//----------You can change PWM parametes below if required-----------------------------------------------------
/* Setting PWM Properties */
const int PWMFreq = 5000; /* 5 KHz */
const int PWMChannel = 0;
const int PWMResolution = 10;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWMResolution) - 1);

//-----------You can change the following Thermister properties if required---------------------------------------

const float R1 = 9915;   // voltage divider resistor value
const float Beta = 3950;  // Beta value
const float To = 301.15;    // Temperature in Kelvin for 27 degree Celsius
const float Ro = 100000;   // Resistance of Thermistor at 25 degree Celsius

//-----------------------------------------------------------------------------------------------------------------------------
//Below values are for Kalman Filter
#ifdef KalmanFilter
const float e_mea = 2.0;
const float e_est = e_mea;
const float q = 0.01;
#else
const uint8_t Lavg_sample_size = 10;
#endif
