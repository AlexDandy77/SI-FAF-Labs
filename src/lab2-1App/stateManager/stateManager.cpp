#include "stateManager.h"

PressStats pressStats;

void initStateManager()
{
    pressStats.lastDurationMs = 0;
    pressStats.newPressDetected = false;
    pressStats.lastWasShort = false;
    pressStats.totalPresses = 0;
    pressStats.shortCount = 0;
    pressStats.longCount = 0;
    pressStats.totalDurationMs = 0;
}

void registerPress(unsigned long durationMs)
{
    pressStats.lastDurationMs = durationMs;
    pressStats.lastWasShort = (durationMs < SHORT_PRESS_THRESHOLD);
    pressStats.newPressDetected = true;
}

bool consumeNewPress(unsigned long *outDuration, bool *outWasShort)
{
    if (!pressStats.newPressDetected) {
        return false;
    }

    *outDuration = pressStats.lastDurationMs;
    *outWasShort = pressStats.lastWasShort;

    pressStats.totalPresses++;
    pressStats.totalDurationMs += pressStats.lastDurationMs;

    if (pressStats.lastWasShort) {
        pressStats.shortCount++;
    } else {
        pressStats.longCount++;
    }

    pressStats.newPressDetected = false;
    return true;
}

void resetPressStats()
{
    pressStats.totalPresses = 0;
    pressStats.shortCount = 0;
    pressStats.longCount = 0;
    pressStats.totalDurationMs = 0;
}