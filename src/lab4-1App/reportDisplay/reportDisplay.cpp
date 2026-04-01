#include "reportDisplay.h"
#include <Arduino.h>
#include <stdio.h>
#include "../stateManager/stateManager.h"

void reportDisplayInit() {
    // Initialized in main app (srvSerialSetup)
}

void reportDisplayProcess() {
    bool currentState = actuator_get_state();
    char raw = getRawCommand();
    char valid = getValidatedCommand();

    printf("====== Actuator Report ======\r\n");
    printf("Raw Input Cmd: %s\r\n", (raw == '1' ? "ON" : "OFF"));
    printf("Validated Cmd: %s\r\n", (valid == '1' ? "ON" : "OFF"));
    printf("Actuator State: %s\r\n", (currentState ? "ACTIVATED" : "DEACTIVATED"));
    printf("=============================\r\n");
}