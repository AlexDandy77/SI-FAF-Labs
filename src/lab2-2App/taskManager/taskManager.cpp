#include "taskManager.h"
#include "../../ddButton/ddButton.h"
#include "../../ddLed/ddLed.h"
#include "../stateManager/stateManager.h"
#include <Arduino.h>
#include <semphr.h>
#include <stdio.h>

// Pin assignments (same as lab2-1)
#define BUTTON_PIN     2
#define LED_GREEN_PIN  13   // short press indicator
#define LED_RED_PIN    12   // long press indicator
#define LED_YELLOW_PIN 11   // blink counter

// ---- Initialization: create FreeRTOS tasks ----
void initTaskManager()
{
    ddButtonInit(BUTTON_PIN);
    ddLedInit(LED_GREEN_PIN);
    ddLedInit(LED_RED_PIN);
    ddLedInit(LED_YELLOW_PIN);

    xTaskCreate(taskButtonMonitor,  "BtnMon",  TASK1_STACK_WORDS,
                NULL, TASK1_PRIORITY, NULL);
    xTaskCreate(taskStatsBlink,     "StBlink", TASK2_STACK_WORDS,
                NULL, TASK2_PRIORITY, NULL);
    xTaskCreate(taskPeriodicReport, "Report",  TASK3_STACK_WORDS,
                NULL, TASK3_PRIORITY, NULL);
}

// ---- Task 1: Detect press/release, measure duration (period 20 ms) ----
// Same edge-detection logic as lab2-1, wrapped in vTaskDelayUntil() +
// mutex for registerPress() + binary semaphore to signal Task 2.
void taskButtonMonitor(void *pvParameters)
{
    (void)pvParameters;

    bool wasPressed = false;
    unsigned long pressStartMs = 0;

    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        bool isPressed = ddButtonIsPressed(BUTTON_PIN);

        // Rising edge: button just pressed
        if (isPressed && !wasPressed) {
            pressStartMs = millis();
            ddLedOff(LED_GREEN_PIN);
            ddLedOff(LED_RED_PIN);
        }

        // Falling edge: button just released
        if (!isPressed && wasPressed) {
            unsigned long duration = millis() - pressStartMs;

            // Critical section: update shared state under mutex
            if (xSemaphoreTake(statsMutex, portMAX_DELAY) == pdTRUE) {
                registerPress(duration);
                xSemaphoreGive(statsMutex);
            }

            // Signal Task 2 that a new press is available
            xSemaphoreGive(newPressSemaphore);

            // Light the appropriate LED (no mutex needed — only Task 1 uses these)
            if (duration < SHORT_PRESS_THRESHOLD) {
                ddLedOn(LED_GREEN_PIN);
            } else {
                ddLedOn(LED_RED_PIN);
            }
        }

        wasPressed = isPressed;
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TASK1_PERIOD_MS));
    }
}

// ---- Task 2: Consume press, update counters, blink yellow LED ----
// Blocks on binary semaphore (event-driven, like lab2-1 ready flag).
// After consuming the press under mutex, performs the blink sequence
// using vTaskDelay() for timing (same 100 ms toggle as lab2-1).
void taskStatsBlink(void *pvParameters)
{
    (void)pvParameters;

    for (;;) {
        // Block until Task 1 signals a new press
        if (xSemaphoreTake(newPressSemaphore, portMAX_DELAY) == pdTRUE) {
            unsigned long duration;
            bool wasShort;

            // Consume press under mutex
            if (xSemaphoreTake(statsMutex, portMAX_DELAY) == pdTRUE) {
                consumeNewPress(&duration, &wasShort);
                xSemaphoreGive(statsMutex);
            }

            // 5 blinks for short, 10 blinks for long (same as lab2-1)
            int blinks = wasShort ? 5 : 10;

            ddLedOff(LED_YELLOW_PIN);
            for (int i = 0; i < blinks; i++) {
                ddLedOn(LED_YELLOW_PIN);
                vTaskDelay(pdMS_TO_TICKS(TASK2_PERIOD_MS / 2));  // 50 ms on
                ddLedOff(LED_YELLOW_PIN);
                vTaskDelay(pdMS_TO_TICKS(TASK2_PERIOD_MS / 2));  // 50 ms off
            }
        }
    }
}

// ---- Task 3: Periodic report via STDIO (every 10 s) ----
// Same report format as lab2-1. Uses vTaskDelayUntil() for precise
// 10-second periods. Snapshot + reset under mutex.
void taskPeriodicReport(void *pvParameters)
{
    (void)pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TASK3_PERIOD_MS));

        // Snapshot stats under mutex, then reset
        unsigned int total, shortCnt, longCnt;
        unsigned long totalMs;

        if (xSemaphoreTake(statsMutex, portMAX_DELAY) == pdTRUE) {
            total    = pressStats.totalPresses;
            shortCnt = pressStats.shortCount;
            longCnt  = pressStats.longCount;
            totalMs  = pressStats.totalDurationMs;
            resetPressStats();
            xSemaphoreGive(statsMutex);
        }

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
    }
}
