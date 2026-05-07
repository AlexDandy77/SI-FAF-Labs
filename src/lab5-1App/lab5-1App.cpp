#include "lab5-1App.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "taskManager/taskManager.h"
#include "../srvSerialStdio/srvSerialStdio.h"

void lab5_1AppSetup() {
    srvSerialSetup(9600); // Initialize printf support over Serial
    printf("Lab 5-1: ON-OFF Control with Hysteresis (Variant A)\r\n");
    srvSerialFlush(); // Ensure serial is fully flushed
    delay(10); // small grace period
    taskManagerInit();
}

void lab5_1AppLoop() {
    // Empty, FreeRTOS manages tasks
}
