#include "ddServo.h"
#include <Arduino.h>
#include <Servo.h>

static Servo deviceServo;

void ddServoInit(int pin) {
    deviceServo.attach(pin);
    deviceServo.write(0); // Start at 0 degrees
}

void ddServoSetAngle(int angle) {
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    deviceServo.write(angle);
}
