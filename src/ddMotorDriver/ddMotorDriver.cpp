#include "ddMotorDriver.h"
#include <Arduino.h>

static int motorForwardPin = -1;
static int motorReversePin = -1;

void ddMotorDriverInit(int forwardPin, int reversePin) {
    motorForwardPin = forwardPin;
    motorReversePin = reversePin;

    pinMode(motorForwardPin, OUTPUT);
    pinMode(motorReversePin, OUTPUT);
    ddMotorDriverStop();
}

void ddMotorDriverSetPercent(int speedPercent) {
    if (motorForwardPin == -1 || motorReversePin == -1) {
        return;
    }

    if (speedPercent < 0) {
        speedPercent = 0;
    }
    if (speedPercent > 100) {
        speedPercent = 100;
    }

    int pwm = (speedPercent * 255) / 100;
    digitalWrite(motorReversePin, LOW);
    analogWrite(motorForwardPin, pwm);
}

void ddMotorDriverStop() {
    if (motorForwardPin == -1 || motorReversePin == -1) {
        return;
    }

    digitalWrite(motorForwardPin, LOW);
    digitalWrite(motorReversePin, LOW);
}
