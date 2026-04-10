#include "stateManager.h"

static float currentTemp = 0.0f;
static float setPointTemp = 30.0f;
static const float hysteresis = 1.5f;
static bool tempValid = false;
static bool relayState = true;
static bool fanState = false;
static int fanSpeedPercent = 0;

void updateCurrentTemp(float temp) { currentTemp = temp; }
float getCurrentTemp() { return currentTemp; }

void updateSetPointTemp(float sp) { setPointTemp = sp; }
float getSetPointTemp() { return setPointTemp; }

void updateRelayState(bool state) { relayState = state; }
bool getRelayState() { return relayState; }

void updateTempValid(bool valid) { tempValid = valid; }
bool isTempValid() { return tempValid; }

void updateFanState(bool state) { fanState = state; }
bool getFanState() { return fanState; }

void updateFanSpeedPercent(int speedPercent) { fanSpeedPercent = speedPercent; }
int getFanSpeedPercent() { return fanSpeedPercent; }

float getHysteresis() { return hysteresis; }
