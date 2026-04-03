#include "reportDisplay.h"
#include "../stateManager/stateManager.h"
#include "../../ddLcd/ddLcd.h"
#include <Arduino.h>
#include <stdio.h>

void reportDisplayInit()
{
    // ddLcdInit() already called in lab4-2App setup.
}

void reportDisplayProcess()
{
    bool relayOn  = getBinaryActuatorOn();
    int  speed    = getRampedSpeed();
    int  cond     = getConditionedTarget();
    bool overload = getAnalogOverload();
    char rawBin   = getRawBinaryCmd();
    char valBin   = getValidatedBinaryCmd();
    int  rawSpeed = getRawAnalogTarget();

    // Serial report (stdout = serial)
    printf("====== Dual Actuator Report ======\r\n");
    printf("BINARY  raw: %s | validated: %s | relay: %s\r\n",
           rawBin == '1' ? "ON " : "OFF",
           valBin == '1' ? "ON " : "OFF",
           relayOn       ? "ON " : "OFF");
    printf("ANALOG  raw: %3d | cond: %3d | servo: %3d\r\n",
           rawSpeed, cond, speed);
    printf("ALERT   overload: %s\r\n", overload ? "YES" : "NO");
    printf("==================================\r\n");

    // LCD report — use fputc/fputs; more reliable than fprintf on AVR FILE streams
    // Line 0 (16 chars): "RLY:ON  SRV:255 "
    // Line 1 (16 chars): "COND:255 OVL:NO "
    char line0[17], line1[17];
    snprintf(line0, sizeof(line0), "RLY:%-3s SRV:%03d",
             relayOn ? "ON" : "OFF", speed);
    snprintf(line1, sizeof(line1), "COND:%03d OVL:%-3s",
             cond, overload ? "YES" : "NO");

    fputc('\f', &lcdOutput);  // clear + cursor to (0,0)
    fputs(line0, &lcdOutput); // write line 0
    fputc('\n', &lcdOutput);  // cursor to (0,1)
    fputs(line1, &lcdOutput); // write line 1
}
