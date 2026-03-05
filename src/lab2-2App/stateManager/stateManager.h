#ifndef STATE_MANAGER_2_2_H
#define STATE_MANAGER_2_2_H

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#define SHORT_PRESS_THRESHOLD 500

// Shared state for button press statistics (same struct as lab2-1)
struct PressStats {
    unsigned long lastDurationMs;     // duration of the most recent press
    bool newPressDetected;            // flag: Task 1 recorded, Task 2 not yet consumed
    bool lastWasShort;                // true if last press < 500 ms

    unsigned int totalPresses;        // total press count in current window
    unsigned int shortCount;          // number of short presses
    unsigned int longCount;           // number of long presses
    unsigned long totalDurationMs;    // sum of all press durations
};

extern PressStats pressStats;

// FreeRTOS synchronization primitives
extern SemaphoreHandle_t statsMutex;         // mutex: protects pressStats
extern SemaphoreHandle_t newPressSemaphore;  // binary semaphore: Task 1 → Task 2 event

void initStateManager();

// Called by Task 1 when button is released (caller must hold statsMutex)
void registerPress(unsigned long durationMs);

// Called by Task 2 — consumes the press, updates counters (caller must hold statsMutex)
// Returns true if a new press was available
bool consumeNewPress(unsigned long *outDuration, bool *outWasShort);

// Called by Task 3 — snapshots and resets statistics (caller must hold statsMutex)
void resetPressStats();

#endif
