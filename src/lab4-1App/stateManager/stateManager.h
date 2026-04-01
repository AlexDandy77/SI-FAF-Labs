#ifndef LAB4_1_STATE_MANAGER_H
#define LAB4_1_STATE_MANAGER_H

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

void stateManagerInit();

void setRawCommand(char cmd);
char getRawCommand();

void setValidatedCommand(char cmd);
char getValidatedCommand();

void setActuatorState(bool state);
bool actuator_get_state();

#endif