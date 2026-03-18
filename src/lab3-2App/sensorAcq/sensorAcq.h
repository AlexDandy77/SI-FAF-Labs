#ifndef SENSOR_ACQ_H
#define SENSOR_ACQ_H

#include <Arduino.h>

// NTC thermistor parameters (B-parameter model)
// Wokwi NTC sensor: VCC -> 10k pullup -> OUT -> NTC -> GND
#define NTC_BETA         3950      // Beta coefficient
#define NTC_R0           10000.0f  // Resistance at T0 (10 kOhm)
#define NTC_T0_KELVIN    298.15f   // Reference temperature (25 C in Kelvin)
#define SERIES_RESISTOR  10000.0f  // Series resistor in voltage divider (10 kOhm)

void  sensorInit(int pin);
int   sensorReadRaw(int pin);
float sensorRawToTemperature(int rawAdc);

#endif
