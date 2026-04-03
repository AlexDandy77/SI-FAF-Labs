#ifndef LAB4_2_STATE_MANAGER_H
#define LAB4_2_STATE_MANAGER_H

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

void stateManagerInit();

// Binary actuator
void     setRawBinaryCmd(char cmd);
char     getRawBinaryCmd();
void     setValidatedBinaryCmd(char cmd);
char     getValidatedBinaryCmd();
void     setBinaryActuatorOn(bool on);
bool     getBinaryActuatorOn();

// Analog actuator
void     setRawAnalogTarget(int val);
int      getRawAnalogTarget();
void     setConditionedTarget(int val);
int      getConditionedTarget();
void     setRampedSpeed(int val);
int      getRampedSpeed();
void     setAnalogOverload(bool flag);
bool     getAnalogOverload();

// Exposed for reportDisplay (matches lab4-1 naming convention)
bool     actuator_get_state();
int      actuator_get_speed();

#endif
