#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "lab4-1App.h"
#include "stateManager/stateManager.h"
#include "taskManager/taskManager.h"
#include "../srvSerialStdio/srvSerialStdio.h"

void lab4_1AppSetup() {
    srvSerialSetup(9600);
    
    printf("Starting Lab 4-1: Binary Actuator Control\r\n");

    stateManagerInit();
    taskManagerInit();
}

void lab4_1AppLoop() {
    // Empty, FreeRTOS tasks handle everything
}