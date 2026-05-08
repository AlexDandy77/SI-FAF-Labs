#ifndef LAB6_1_FSM_H
#define LAB6_1_FSM_H

#include <Arduino.h>

enum LedFsmState {
    LED_FSM_OFF = 0,
    LED_FSM_ON = 1
};

LedFsmState ledFsmGetInitialState();
LedFsmState ledFsmNextState(LedFsmState currentState, uint8_t input);
bool ledFsmOutputForState(LedFsmState state);
const char* ledFsmStateName(LedFsmState state);
uint16_t ledFsmDelayMs(LedFsmState state);

#endif
