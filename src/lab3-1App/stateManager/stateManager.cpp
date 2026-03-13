#include "stateManager.h"

SensorState sensorState;
SemaphoreHandle_t stateMutex = NULL;

void initStateManager()
{
    stateMutex = xSemaphoreCreateMutex();

    sensorState.tempRawAdc            = 0;
    sensorState.temperature           = 0.0f;
    sensorState.tempAlertState        = ALERT_NORMAL;
    sensorState.tempDebounceCounter   = 0;

    sensorState.lightDigital          = 0;
    sensorState.lightAlertState       = ALERT_NORMAL;
    sensorState.lightDebounceCounter  = 0;
}
