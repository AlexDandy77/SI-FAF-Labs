#include "taskManager.h"
#include "../../ddButton/ddButton.h"
#include "../../ddLed/ddLed.h"
#include "../stateManager/stateManager.h"
#include <Arduino.h>
#include <stdio.h>

#define BUTTON_PIN 2
#define LED_GREEN_PIN 13   // short press indicator
#define LED_RED_PIN 12     // long press indicator
#define LED_YELLOW_PIN 11  // blink counter

// ---- Task table: { recurrence, offset, function, counter, ready } ----
TaskContext taskTable[NUM_TASKS] = {
    { 20,    0,  taskButtonMonitor,   20,    false },  // Task 1: every 20 ms
    { 100,   10, taskStatsBlink,      110,   false },  // Task 2: every 100 ms
    { 10000, 50, taskPeriodicReport,  10050, false }   // Task 3: every 10 s
};

void initTaskManager()
{
    ddButtonInit(BUTTON_PIN);
    ddLedInit(LED_GREEN_PIN);
    ddLedInit(LED_RED_PIN);
    ddLedInit(LED_YELLOW_PIN);

    // initialise counters with recurrence + offset
    for (int i = 0; i < NUM_TASKS; i++) {
        taskTable[i].counter = taskTable[i].recurrence + taskTable[i].offset;
        taskTable[i].ready = false;
    }
}

// Non-preemptive scheduler: execute ONE ready task per call
void runScheduler()
{
    for (int i = 0; i < NUM_TASKS; i++) {
        if (taskTable[i].ready) {
            taskTable[i].ready = false;
            taskTable[i].taskFunction();
            break;  // one task per tick
        }
    }
}

// ---- Task 1: Detect press/release and measure duration ----
void taskButtonMonitor()
{
    static bool wasPressed = false;
    static unsigned long pressStartMs = 0;

    bool isPressed = ddButtonIsPressed(BUTTON_PIN);

    // rising edge: button just pressed
    if (isPressed && !wasPressed) {
        pressStartMs = millis();
        // turn off previous indicator LEDs
        ddLedOff(LED_GREEN_PIN);
        ddLedOff(LED_RED_PIN);
    }

    // falling edge: button just released
    if (!isPressed && wasPressed) {
        unsigned long duration = millis() - pressStartMs;
        registerPress(duration);

        // signal short / long with the appropriate LED
        if (duration < SHORT_PRESS_THRESHOLD) {
            ddLedOn(LED_GREEN_PIN);
        } else {
            ddLedOn(LED_RED_PIN);
        }
    }

    wasPressed = isPressed;
}

// ---- Task 2: Consume press, update counters, blink yellow LED ----
void taskStatsBlink()
{
    static int blinksRemaining = 0;

    // check for a new press and consume it
    unsigned long duration;
    bool wasShort;
    if (consumeNewPress(&duration, &wasShort)) {
        ddLedOff(LED_YELLOW_PIN);
        // 5 blinks (10 toggles) for short, 10 blinks (20 toggles) for long
        blinksRemaining = wasShort ? (5 * 2) : (10 * 2);
    }

    // handle yellow LED blinking
    if (blinksRemaining > 0) {
        ddLedToggle(LED_YELLOW_PIN);
        blinksRemaining--;
    } else {
        ddLedOff(LED_YELLOW_PIN);
    }
}

// ---- Task 3: Periodic report via STDIO (every 10 s) ----
void taskPeriodicReport()
{
    unsigned int total    = pressStats.totalPresses;
    unsigned int shortCnt = pressStats.shortCount;
    unsigned int longCnt  = pressStats.longCount;
    unsigned long totalMs = pressStats.totalDurationMs;

    unsigned int avgMs = 0;
    if (total > 0) {
        avgMs = (unsigned int)(totalMs / total);
    }

    printf("=== Report (10 s) ===\r\n");
    printf("Total presses:    %u\r\n", total);
    printf("Short (< 500 ms): %u\r\n", shortCnt);
    printf("Long (>= 500 ms): %u\r\n", longCnt);
    printf("Avg duration:     %u ms\r\n", avgMs);
    printf("=====================\r\n\r\n");

    resetPressStats();
}