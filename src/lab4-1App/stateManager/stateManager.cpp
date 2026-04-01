#include "stateManager.h"
#include <Arduino.h>

static char rawCommand = '0';
static char validatedCommand = '0';
static bool actuatorState = false;

static SemaphoreHandle_t stateMutex = NULL;

void stateManagerInit() {
    stateMutex = xSemaphoreCreateMutex();
}

void setRawCommand(char cmd) {
    if (stateMutex != NULL && xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        rawCommand = cmd;
        xSemaphoreGive(stateMutex);
    }
}

char getRawCommand() {
    char cmd = '0';
    if (stateMutex != NULL && xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        cmd = rawCommand;
        xSemaphoreGive(stateMutex);
    }
    return cmd;
}

void setValidatedCommand(char cmd) {
    if (stateMutex != NULL && xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        validatedCommand = cmd;
        xSemaphoreGive(stateMutex);
    }
}

char getValidatedCommand() {
    char cmd = '0';
    if (stateMutex != NULL && xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        cmd = validatedCommand;
        xSemaphoreGive(stateMutex);
    }
    return cmd;
}

void setActuatorState(bool state) {
    if (stateMutex != NULL && xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        actuatorState = state;
        xSemaphoreGive(stateMutex);
    }
}

bool actuator_get_state() {
    bool state = false;
    if (stateMutex != NULL && xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        state = actuatorState;
        xSemaphoreGive(stateMutex);
    }
    return state;
}