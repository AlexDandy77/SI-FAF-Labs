#ifndef LAB5_1_STATEMANAGER_H
#define LAB5_1_STATEMANAGER_H
#include <stdbool.h>

void updateCurrentTemp(float temp);
float getCurrentTemp();

void updateSetPointTemp(float sp);
float getSetPointTemp();

void updateRelayState(bool param_state);
bool getRelayState();

void updateTempValid(bool valid);
bool isTempValid();

void updateFanState(bool state);
bool getFanState();

void updateFanSpeedPercent(int speedPercent);
int getFanSpeedPercent();

float getHysteresis();

#endif
