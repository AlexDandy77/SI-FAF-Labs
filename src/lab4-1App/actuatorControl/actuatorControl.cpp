#include "actuatorControl.h"
#include <Arduino.h>
#include "../stateManager/stateManager.h"

#define RELAY_PIN 2

void actuatorControlInit() {
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW); // Default OFF
    setActuatorState(false);
}

void actuatorControlProcess() {
    char validCmd = getValidatedCommand();
    bool targetState = (validCmd == '1' || validCmd == 'o' || validCmd == 'O') ? true : false;
    
    // Actuator logic
    digitalWrite(RELAY_PIN, targetState ? HIGH : LOW);
    
    // Update State
    setActuatorState(targetState);
}