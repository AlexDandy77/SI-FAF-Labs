#include "lab3-1App.h"
#include "../srvSerialStdio/srvSerialStdio.h"
#include "stateManager/stateManager.h"
#include "taskManager/taskManager.h"
#include <Arduino_FreeRTOS.h>

#define BAUD_RATE 9600

void lab3_1AppSetup()
{
    srvSerialSetup(BAUD_RATE);
    initStateManager();
    initTaskManager();

    Serial.print(F("Lab 3-1: Dual Sensor Monitor (NTC + LDR, FreeRTOS)\r\n"));
    Serial.print(F("Temp th: 30 +/- 1 C | Light: digital dark/light | Debounce: 5\r\n\r\n"));

    // FreeRTOS scheduler is started by the library's initVariant()
    // after setup() returns — loop() becomes the idle task.
}

void lab3_1AppLoop()
{
    // Empty — FreeRTOS scheduler manages all tasks.
    // loop() runs as the idle task at the lowest priority.
}
