#include "actuatorControl.h"
#include "../stateManager/stateManager.h"
#include "../../ddLed/ddLed.h"

#define LED_PIN 13

void actuatorControlInit()
{
    ddLedInit(LED_PIN);
}

void actuatorControlProcess()
{
    Lab61Snapshot snapshot;
    stateManagerGetSnapshot(&snapshot);
    ddLedSetState(LED_PIN, snapshot.ledOn);
}
