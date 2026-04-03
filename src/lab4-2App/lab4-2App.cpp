#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "lab4-2App.h"
#include "stateManager/stateManager.h"
#include "taskManager/taskManager.h"
#include "../srvSerialStdio/srvSerialStdio.h"
#include "../ddLcd/ddLcd.h"
#include "../ddKeypad/ddKeypad.h"

void lab4_2AppSetup()
{
    // 1. Serial stdio (sets stdout = serial, stdin = serial)
    srvSerialSetup(9600);

    // 2. Save serial stdout before LCD init overrides it
    FILE *serialStdout = stdout;

    // 3. Initialise LCD hardware and lcdOutput FILE stream
    ddLcdInit();

    // 4. Restore stdout to serial so printf() writes to terminal
    stdout = serialStdout;

    // 5. Redirect stdin to keypad (overrides serial stdin)
    ddKeypadInit();

    stateManagerInit();
    taskManagerInit();

    printf("Lab 4-2: Dual Actuator Control (Variant C)\r\n");
    printf("Binary relay  -> serial: '1'=ON  '0'=OFF\r\n");
    printf("Analog speed  -> keypad: <0-255> then '#'\r\n");
    printf("                         '*' clears input\r\n");
}

void lab4_2AppLoop()
{
    // Empty — FreeRTOS scheduler owns the CPU.
}
