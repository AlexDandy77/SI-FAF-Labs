#include "stateManager.h"

static float currentTemp = 0.0f;
static float setPointTemp = 30.0f;
static int pidOutputPwm = 0;
static bool tempValid = false;
static int heaterPwm = 0;
static bool relayState = false;
static bool fanState = false;
static int fanSpeedPercent = 0;

static float Kp = 50.0f;
static float Ki = 2.0f;
static float Kd = 10.0f;

void updateCurrentTemp(float temp) { currentTemp = temp; }
float getCurrentTemp() { return currentTemp; }

void updateSetPointTemp(float sp) { setPointTemp = sp; }
float getSetPointTemp() { return setPointTemp; }

void updatePidOutput(int pwm) { pidOutputPwm = pwm; }
int getPidOutput() { return pidOutputPwm; }

void updateTempValid(bool valid) { tempValid = valid; }
bool isTempValid() { return tempValid; }

void updateHeaterPwm(int pwm) { heaterPwm = pwm; }
int getHeaterPwm() { return heaterPwm; }

void updateRelayState(bool state) { relayState = state; }
bool getRelayState() { return relayState; }

void updateFanState(bool state) { fanState = state; }
bool getFanState() { return fanState; }

void updateFanSpeedPercent(int speedPercent) { fanSpeedPercent = speedPercent; }
int getFanSpeedPercent() { return fanSpeedPercent; }

void setPidConstants(float p, float i, float d) { Kp = p; Ki = i; Kd = d; }
void getPidConstants(float* p, float* i, float* d) { *p = Kp; *i = Ki; *d = Kd; }
