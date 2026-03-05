#include "lab2-2App.h"
#include "srvSerialStdio/srvSerialStdio.h"
#include "stateManager/stateManager.h"
#include "taskManager/taskManager.h"
#include <Arduino_FreeRTOS.h>

#define BAUD_RATE 9600

void lab2_2AppSetup()
{
    srvSerialSetup(BAUD_RATE);
    initStateManager();
    initTaskManager();

    Serial.print(F("Lab 2-2: Button Duration Monitor (FreeRTOS)\r\n"));
    Serial.print(F("Press the button. Reports every 10 s.\r\n\r\n"));

    // FreeRTOS scheduler is started by the library's initVariant()
    // after setup() returns — loop() becomes the idle task.
}

void lab2_2AppLoop()
{
    // Empty — FreeRTOS scheduler manages all tasks.
    // loop() runs as the idle task at the lowest priority.
}
