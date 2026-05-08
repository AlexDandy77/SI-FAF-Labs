#include "buttonInput.h"
#include "../stateManager/stateManager.h"
#include "../../ddButton/ddButton.h"
#include <Arduino_FreeRTOS.h>

#define BUTTON_PIN 2
#define BUTTON_LOCKOUT_MS 80

static bool initialized = false;
static bool wasPressed = false;
static bool clickStarted = false;
static TickType_t lastEventTick = 0;

void buttonInputInit()
{
    ddButtonInit(BUTTON_PIN);

    wasPressed = ddButtonIsPressed(BUTTON_PIN) != 0;
    clickStarted = wasPressed;
    lastEventTick = xTaskGetTickCount() - pdMS_TO_TICKS(BUTTON_LOCKOUT_MS);

    initialized = true;

    stateManagerSetButtonPressed(wasPressed);
}

bool buttonInputPollPressEvent()
{
    if (!initialized) {
        buttonInputInit();
    }

    bool rawPressed = ddButtonIsPressed(BUTTON_PIN) != 0;
    TickType_t now = xTaskGetTickCount();
    bool previousPressed = wasPressed;

    if (rawPressed &&
        !previousPressed) {
        clickStarted = true;
    }

    if (!rawPressed &&
        previousPressed &&
        clickStarted &&
        (now - lastEventTick) >= pdMS_TO_TICKS(BUTTON_LOCKOUT_MS)) {
        lastEventTick = now;
        clickStarted = false;
        wasPressed = rawPressed;
        stateManagerSetButtonPressed(wasPressed);
        return true;
    }

    if (rawPressed != wasPressed) {
        wasPressed = rawPressed;
        stateManagerSetButtonPressed(wasPressed);
    }

    return false;
}
