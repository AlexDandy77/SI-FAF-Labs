#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <Arduino.h>

#define SHORT_PRESS_THRESHOLD 500

// Shared state for button press statistics
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

void initStateManager();

// Called by Task 1 when button is released
void registerPress(unsigned long durationMs);

// Called by Task 2 — consumes the press, updates counters
// Returns true if a new press was available
bool consumeNewPress(unsigned long *outDuration, bool *outWasShort);

// Called by Task 3 — resets statistics after reporting
void resetPressStats();

#endif