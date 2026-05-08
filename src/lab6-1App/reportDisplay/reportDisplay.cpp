#include "reportDisplay.h"
#include "../fsm/fsm.h"
#include "../stateManager/stateManager.h"
#include <Arduino_FreeRTOS.h>
#include <stdio.h>

#define REPORT_PERIOD_MS 500

void reportDisplayInit()
{
}

void reportDisplayPrintCurrentState()
{
    Lab61Snapshot snapshot;
    stateManagerGetSnapshot(&snapshot);

    printf("State=%s,LED=%s,Button=%s,Transitions=%lu,LastTransitionMs=%lu\r\n",
           ledFsmStateName(snapshot.state),
           snapshot.ledOn ? "ON" : "OFF",
           snapshot.buttonPressed ? "PRESSED" : "RELEASED",
           snapshot.transitionCount,
           snapshot.lastTransitionMs);
}

void taskReportDisplay(void* pvParameters)
{
    (void)pvParameters;
    TickType_t lastWake = xTaskGetTickCount();

    reportDisplayPrintCurrentState();

    for (;;) {
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(REPORT_PERIOD_MS));
        reportDisplayPrintCurrentState();
    }
}
