#include "reportDisplay.h"
#include "../stateManager/stateManager.h"
#include <stdio.h>

void reportDisplayInit() {
}

void reportDisplayProcess() {
    float sp = getSetPointTemp();
    float temp = getCurrentTemp();
    int pid = getPidOutput();
    int heaterPwm = getHeaterPwm();
    int fanSpeed = getFanSpeedPercent();
    bool fanOn = getFanState();
    bool relayOn = getRelayState();

    int sp_int = (int)sp;
    int sp_frac = (int)((sp - sp_int) * 100);
    if (sp_frac < 0) {
        sp_frac = -sp_frac;
    }
    
    int t_int = (int)temp;
    int t_frac = (int)((temp - t_int) * 100);
    if (t_frac < 0) {
        t_frac = -t_frac;
    }

    printf(">Temp:");
    if (!isTempValid()) {
        printf("nan");
    } else {
        printf("%d.%02d", t_int, t_frac);
    }
    printf(",SetPoint:%d.%02d,PID:%d,HeaterPwm:%d,FanSpeed:%d,Fan:%d,Relay:%d\r\n",
           sp_int,
           sp_frac,
           pid,
           heaterPwm,
           fanSpeed,
           fanOn ? 1 : 0,
           relayOn ? 1 : 0);
}
