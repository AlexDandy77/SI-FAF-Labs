#include "ddRelay.h"
#include <Arduino.h>

void ddRelayInit(int pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void ddRelaySet(int pin, bool on) {
    digitalWrite(pin, on ? HIGH : LOW);
}
