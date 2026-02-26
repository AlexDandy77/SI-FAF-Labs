#include "srvSerialStdio/srvSerialStdio.h"
#include "stateManager/stateManager.h"
#include "sysTimer/sysTimer.h"
#include "taskManager/taskManager.h"

#define BAUD_RATE 9600

void lab2_1AppSetup()
{
    srvSerialSetup(BAUD_RATE);
    initStateManager();
    initTaskManager();
    initSystemTimer(); // start timer last, after all peripherals are ready

    printf("Lab 2-1: Button Duration Monitor\r\n");
    printf("Press the button. Reports every 10 s.\r\n\r\n");
}

void lab2_1AppLoop()
{
    // non-preemptive scheduler: runs one ready task per call
    runScheduler();
}