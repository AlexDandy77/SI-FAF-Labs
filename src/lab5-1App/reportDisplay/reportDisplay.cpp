#include "reportDisplay.h"
#include "../stateManager/stateManager.h"
#include <Arduino.h>

void reportDisplayInit() {
}

void reportDisplayProcess() {
    float sp = getSetPointTemp();
    float temp = getCurrentTemp();
    float hysteresis = getHysteresis();
    float vOn = sp + hysteresis;
    float vOff = sp - hysteresis;
    bool fanOn = getFanState();
    int fanSpeed = getFanSpeedPercent();
    bool heaterOn = getRelayState();

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

    int v_on_int = (int)vOn;
    int v_off_int = (int)vOff;

    Serial.print(">Temp:");
    if (!isTempValid()) {
        Serial.print("nan");
    } else {
        Serial.print(t_int);
        Serial.print('.');
        if (t_frac < 10) {
            Serial.print('0');
        }
        Serial.print(t_frac);
    }
    Serial.print(",SetPoint:");
    Serial.print(sp_int);
    Serial.print('.');
    if (sp_frac < 10) {
        Serial.print('0');
    }
    Serial.print(sp_frac);
    Serial.print(",V_on:");
    Serial.print(v_on_int);
    Serial.print(",V_off:");
    Serial.print(v_off_int);
    Serial.print(",Fan:");
    Serial.print(fanOn ? 1 : 0);
    Serial.print(",Speed:");
    Serial.print(fanSpeed);
    Serial.print(",Heater:");
    Serial.println(heaterOn ? 1 : 0);
}
