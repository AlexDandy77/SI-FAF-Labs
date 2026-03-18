#include "lab3-2App.h"
#include "../srvSerialStdio/srvSerialStdio.h"
#include "stateManager/stateManager.h"
#include "taskManager/taskManager.h"
#include <Arduino_FreeRTOS.h>

#define BAUD_RATE 9600

void lab3_2AppSetup()
{
    srvSerialSetup(BAUD_RATE);
    initStateManager();
    initTaskManager();

    Serial.print(F("Lab 3-2: Analog Signal Conditioning (NTC + LDR, FreeRTOS)\r\n"));
    Serial.print(F("Conditioning: Saturation(100-900), Median(5), EMA(a=0.2)\r\n\r\n"));

    // FreeRTOS scheduler is started by the library's initVariant()
    // after setup() returns — loop() becomes the idle task.
}

void lab3_2AppLoop()
{
    // Empty — FreeRTOS scheduler manages all tasks.
    // loop() runs as the idle task at the lowest priority.
}
