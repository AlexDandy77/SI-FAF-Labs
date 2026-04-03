#include "actuatorControl.h"
#include <Arduino.h>
#include "../stateManager/stateManager.h"
#include "../../ddRelay/ddRelay.h"

#define RELAY_PIN 2

void actuatorControlInit() {
    ddRelayInit(RELAY_PIN);
    setActuatorState(false);
}

void actuatorControlProcess() {
    char validCmd = getValidatedCommand();
    bool targetState = (validCmd == '1' || validCmd == 'o' || validCmd == 'O') ? true : false;
    
    // Actuator logic
    ddRelaySet(RELAY_PIN, targetState);
    
    // Update State
    setActuatorState(targetState);
}