#include "stateManager.h"

static SemaphoreHandle_t stateMutex = NULL;
static Lab61Snapshot currentSnapshot;

void stateManagerInit()
{
    stateMutex = xSemaphoreCreateMutex();

    currentSnapshot.state = ledFsmGetInitialState();
    currentSnapshot.ledOn = ledFsmOutputForState(currentSnapshot.state);
    currentSnapshot.buttonPressed = false;
    currentSnapshot.lastEventWasPress = false;
    currentSnapshot.transitionCount = 0;
    currentSnapshot.lastTransitionMs = 0;
}

void stateManagerSetButtonPressed(bool pressed)
{
    if (stateMutex == NULL) {
        return;
    }

    if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        currentSnapshot.buttonPressed = pressed;
        xSemaphoreGive(stateMutex);
    }
}

void stateManagerSetFsmState(LedFsmState state, bool pressEvent)
{
    if (stateMutex == NULL) {
        return;
    }

    if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        currentSnapshot.state = state;
        currentSnapshot.ledOn = ledFsmOutputForState(state);
        currentSnapshot.lastEventWasPress = pressEvent;
        if (pressEvent) {
            currentSnapshot.transitionCount++;
            currentSnapshot.lastTransitionMs = millis();
        }
        xSemaphoreGive(stateMutex);
    }
}

void stateManagerGetSnapshot(Lab61Snapshot* snapshot)
{
    if (snapshot == NULL) {
        return;
    }

    if (stateMutex == NULL) {
        *snapshot = currentSnapshot;
        return;
    }

    if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        *snapshot = currentSnapshot;
        xSemaphoreGive(stateMutex);
    }
}
