#include "lab6-1App.h"
#include "stateManager/stateManager.h"
#include "taskManager/taskManager.h"
#include "../srvSerialStdio/srvSerialStdio.h"
#include <Arduino.h>
#include <stdio.h>

void lab6_1AppSetup()
{
    srvSerialSetup(9600);
    printf("Lab 6-1: FSM Button-LED Control\r\n");
    printf("Press the button to toggle the LED state.\r\n");
    srvSerialFlush();
    delay(10);

    stateManagerInit();

    Lab61Snapshot snapshot;
    stateManagerGetSnapshot(&snapshot);
    printf("Initial state: %s, LED=%s\r\n",
           ledFsmStateName(snapshot.state),
           snapshot.ledOn ? "ON" : "OFF");

    if (taskManagerInit()) {
        printf("Lab 6-1 tasks created successfully.\r\n");
    }

    srvSerialFlush();
}

void lab6_1AppLoop()
{
    // FreeRTOS manages all lab tasks.
}
