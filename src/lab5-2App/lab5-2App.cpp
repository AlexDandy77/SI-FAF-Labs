#include "lab5-2App.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "taskManager/taskManager.h"
#include "../srvSerialStdio/srvSerialStdio.h"

void lab5_2AppSetup() {
    srvSerialSetup(9600); // Initialize printf support over Serial
    printf("Lab 5-2: PID Control (Variant A)\r\n");
    srvSerialFlush();
    delay(10);
    taskManagerInit();
}

void lab5_2AppLoop() {
    // Empty, FreeRTOS handles tasks
}
