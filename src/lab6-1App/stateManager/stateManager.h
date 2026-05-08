#ifndef LAB6_1_STATE_MANAGER_H
#define LAB6_1_STATE_MANAGER_H

#include "../fsm/fsm.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

struct Lab61Snapshot {
    LedFsmState state;
    bool ledOn;
    bool buttonPressed;
    bool lastEventWasPress;
    unsigned long transitionCount;
    unsigned long lastTransitionMs;
};

void stateManagerInit();
void stateManagerSetButtonPressed(bool pressed);
void stateManagerSetFsmState(LedFsmState state, bool pressEvent);
void stateManagerGetSnapshot(Lab61Snapshot* snapshot);

#endif
