#include "stateManager.h"

PressStats pressStats;

SemaphoreHandle_t statsMutex        = NULL;
SemaphoreHandle_t newPressSemaphore = NULL;

void initStateManager()
{
    // Create FreeRTOS synchronization objects
    statsMutex        = xSemaphoreCreateMutex();
    newPressSemaphore = xSemaphoreCreateBinary();

    // Initialize shared state (same as lab2-1)
    pressStats.lastDurationMs   = 0;
    pressStats.newPressDetected = false;
    pressStats.lastWasShort     = false;
    pressStats.totalPresses     = 0;
    pressStats.shortCount       = 0;
    pressStats.longCount        = 0;
    pressStats.totalDurationMs  = 0;
}

// Identical logic to lab2-1; caller must hold statsMutex
void registerPress(unsigned long durationMs)
{
    pressStats.lastDurationMs  = durationMs;
    pressStats.lastWasShort    = (durationMs < SHORT_PRESS_THRESHOLD);
    pressStats.newPressDetected = true;
}

// Identical logic to lab2-1; caller must hold statsMutex
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

// Identical logic to lab2-1; caller must hold statsMutex
void resetPressStats()
{
    pressStats.totalPresses    = 0;
    pressStats.shortCount      = 0;
    pressStats.longCount       = 0;
    pressStats.totalDurationMs = 0;
}
