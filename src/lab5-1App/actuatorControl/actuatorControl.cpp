#include "actuatorControl.h"
#include "../../ddLed/ddLed.h"
#include "../../ddMotorDriver/ddMotorDriver.h"
#include "../../ddRelay/ddRelay.h"
#include "../stateManager/stateManager.h"

#define RELAY_PIN 2
#define GREEN_LED_PIN 4
#define RED_LED_PIN 5
#define MOTOR_FORWARD_PIN 10
#define MOTOR_REVERSE_PIN 11

void actuatorControlInit() {
    ddRelayInit(RELAY_PIN);
    ddMotorDriverInit(MOTOR_FORWARD_PIN, MOTOR_REVERSE_PIN);
    ddLedInit(GREEN_LED_PIN);
    ddLedInit(RED_LED_PIN);
    ddRelaySet(RELAY_PIN, getRelayState());
    ddMotorDriverStop();
    ddLedOff(GREEN_LED_PIN);
    ddLedOn(RED_LED_PIN);
}

void actuatorControlProcess() {
    bool fanOn = getFanState();
    int fanSpeed = getFanSpeedPercent();

    ddRelaySet(RELAY_PIN, getRelayState());

    if (fanOn && fanSpeed > 0) {
        ddMotorDriverSetPercent(fanSpeed);
    } else {
        ddMotorDriverStop();
    }

    ddLedSetState(GREEN_LED_PIN, fanOn);
    ddLedSetState(RED_LED_PIN, !fanOn);
}
