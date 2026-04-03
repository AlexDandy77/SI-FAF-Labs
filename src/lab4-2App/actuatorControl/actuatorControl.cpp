#include "actuatorControl.h"
#include "../stateManager/stateManager.h"
#include <Arduino.h>
#include "../../ddRelay/ddRelay.h"
#include "../../ddServo/ddServo.h"

#define RELAY_PIN  2
#define SERVO_PIN  3

void actuatorControlInit()
{
    ddRelayInit(RELAY_PIN);
    setBinaryActuatorOn(false);

    ddServoInit(SERVO_PIN);
    setRampedSpeed(0);
}

void actuatorControlProcess()
{
    // Binary relay
    bool on = (getValidatedBinaryCmd() == '1');
    ddRelaySet(RELAY_PIN, on);
    setBinaryActuatorOn(on);

    // Analog servo: map speed [0,255] to angle [0,180]
    int speed = getRampedSpeed();
    int angle = map(speed, 0, 255, 0, 180);
    ddServoSetAngle(angle);
}
