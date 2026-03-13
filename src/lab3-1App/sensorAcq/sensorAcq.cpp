#include "sensorAcq.h"
#include <math.h>

void sensorInit(int pin)
{
    pinMode(pin, INPUT);
}

int sensorReadRaw(int pin)
{
    return analogRead(pin);
}

float sensorRawToTemperature(int rawAdc)
{
    // Clamp to prevent division by zero
    if (rawAdc <= 0)    rawAdc = 1;
    if (rawAdc >= 1023) rawAdc = 1022;

    // Voltage divider: VCC -> R_series(10k) -> OUT -> R_ntc -> GND
    // R_ntc = R_series * ADC / (1023 - ADC)
    float resistance = SERIES_RESISTOR * (float)rawAdc / (1023.0f - (float)rawAdc);

    // Steinhart-Hart B-parameter equation:
    // 1/T = 1/T0 + (1/B) * ln(R/R0)
    float steinhart = log(resistance / NTC_R0) / (float)NTC_BETA;
    steinhart += 1.0f / NTC_T0_KELVIN;
    float tempKelvin = 1.0f / steinhart;

    return tempKelvin - 273.15f;
}
