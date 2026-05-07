#ifndef LAB5_2_STATEMANAGER_H
#define LAB5_2_STATEMANAGER_H

void updateCurrentTemp(float temp);
float getCurrentTemp();

void updateSetPointTemp(float sp);
float getSetPointTemp();

void updatePidOutput(int pwm);
int getPidOutput();

void updateTempValid(bool valid);
bool isTempValid();

void updateHeaterPwm(int pwm);
int getHeaterPwm();

void updateRelayState(bool state);
bool getRelayState();

void updateFanState(bool state);
bool getFanState();

void updateFanSpeedPercent(int speedPercent);
int getFanSpeedPercent();

void setPidConstants(float p, float i, float d);
void getPidConstants(float* p, float* i, float* d);

#endif
