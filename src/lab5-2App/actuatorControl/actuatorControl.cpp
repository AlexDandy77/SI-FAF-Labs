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

#define HEATER_WINDOW_TICKS 20

void actuatorControlInit() {
    ddRelayInit(RELAY_PIN);
    ddMotorDriverInit(MOTOR_FORWARD_PIN, MOTOR_REVERSE_PIN);
    ddLedInit(GREEN_LED_PIN);
    ddLedInit(RED_LED_PIN);

    ddRelaySet(RELAY_PIN, false);
    ddMotorDriverStop();
    ddLedOff(GREEN_LED_PIN);
    ddLedOff(RED_LED_PIN);
}

void actuatorControlProcess() {
    int heaterPwm = getHeaterPwm();
    bool fanOn = getFanState();
    int fanSpeed = getFanSpeedPercent();

    static int heaterTick = 0;
    int ticksOn = (heaterPwm * HEATER_WINDOW_TICKS) / 255;
    bool heaterOn = (heaterPwm > 0) && !fanOn && (heaterTick < ticksOn);

    ddRelaySet(RELAY_PIN, heaterOn);
    updateRelayState(heaterOn);

    if (fanOn && fanSpeed > 0) {
        ddMotorDriverSetPercent(fanSpeed);
    } else {
        ddMotorDriverStop();
    }

    ddLedSetState(GREEN_LED_PIN, fanOn);
    ddLedSetState(RED_LED_PIN, heaterPwm > 0 && !fanOn);

    heaterTick++;
    if (heaterTick >= HEATER_WINDOW_TICKS) {
        heaterTick = 0;
    }
}
