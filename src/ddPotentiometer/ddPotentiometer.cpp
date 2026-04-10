#include "ddPotentiometer.h"

static int potPin = -1;

void ddPotentiometerInit(int pin) {
    potPin = pin;
    pinMode(potPin, INPUT);
}

int ddPotentiometerRead() {
    if (potPin == -1) return 0;
    return analogRead(potPin);
}
