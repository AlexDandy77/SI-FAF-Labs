#include "userInput.h"
#include "../../ddPotentiometer/ddPotentiometer.h"
#include "../stateManager/stateManager.h"
#include <Arduino.h>

#define POT_PIN A0

void userInputInit() {
    ddPotentiometerInit(POT_PIN);
}

void userInputProcess() {
    int raw = ddPotentiometerRead();
    // Map 0-1023 to 15.0C - 35.0C
    float sp = 15.0f + (raw / 1023.0f) * 20.0f;
    updateSetPointTemp(sp);
}
