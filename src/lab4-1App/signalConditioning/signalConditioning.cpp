#include "signalConditioning.h"
#include "../stateManager/stateManager.h"
#include <Arduino.h>

#define DEBOUNCE_THRESHOLD 2

static char lastRawCommand = '0';
static int debounceCounter = 0;

void signalConditioningProcess() {
    char currentRaw = getRawCommand();
    char currentValidated = getValidatedCommand();
    
    // Software Debouncing
    if (currentRaw != currentValidated) {
        if (currentRaw == lastRawCommand) {
            debounceCounter++;
            if (debounceCounter >= DEBOUNCE_THRESHOLD) {
                setValidatedCommand(currentRaw);
                debounceCounter = 0; // Reset counter after successful validation
            }
        } else {
            debounceCounter = 1; // Restart counter with new raw
            lastRawCommand = currentRaw;
        }
    } else {
        debounceCounter = 0; // State is stable
        lastRawCommand = currentRaw;
    }
}