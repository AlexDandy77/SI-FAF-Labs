#include "signalConditioning.h"
#include "../stateManager/stateManager.h"

#define SPEED_LOW 50
#define SPEED_HIGH 100
#define ZONE_OFFSET 3.0f

void signalConditioningInit() {
    // Initial state setup if any
}

void signalConditioningProcess() {
    if (!isTempValid()) {
        updateFanState(false);
        updateFanSpeedPercent(0);
        updateRelayState(true);
        return;
    }

    float temp = getCurrentTemp();
    float sp = getSetPointTemp();
    float hysteresis = getHysteresis();
    float vOn = sp + hysteresis;
    float vOff = sp - hysteresis;
    bool fanOn = getFanState();

    if (temp >= vOn) {
        fanOn = true;
    } else if (temp <= vOff) {
        fanOn = false;
    }

    int speedPercent = 0;
    if (fanOn) {
        speedPercent = (temp >= vOn + ZONE_OFFSET) ? SPEED_HIGH : SPEED_LOW;
    }

    updateFanState(fanOn);
    updateFanSpeedPercent(speedPercent);
    updateRelayState(!fanOn);
}
