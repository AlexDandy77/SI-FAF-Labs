#ifndef STATE_MANAGER_3_2_H
#define STATE_MANAGER_3_2_H

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "../alertManager/alertManager.h"

struct SensorState {
    int        tempRawAdc;            // Temperature sensor raw ADC (0-1023)
    int        tempSaturated;         // Saturated ADC value
    int        tempMedian;            // Median filtered ADC value
    float      tempEmaRaw;            // EMA filtered ADC value
    float      temperature;           // Converted temperature (C)
    AlertState tempAlertState;        // Temperature alert state
    int        tempDebounceCounter;   // Temperature debounce count

    int        lightDigital;          // LDR DO pin state (1 = dark, 0 = light)
    AlertState lightAlertState;       // Light alert state
    int        lightDebounceCounter;  // Light debounce count
};

extern SensorState sensorState;
extern SemaphoreHandle_t stateMutex;

void initStateManager();

#endif
