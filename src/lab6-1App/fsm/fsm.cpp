#include "fsm.h"

struct LedFsmRow {
    const char* name;
    bool ledOn;
    uint16_t delayMs;
    LedFsmState next[2];
};

static const LedFsmRow fsmTable[] = {
    { "LED_OFF", false, 100, { LED_FSM_OFF, LED_FSM_ON } },
    { "LED_ON",  true,  100, { LED_FSM_ON,  LED_FSM_OFF } }
};

static LedFsmState normalizeState(LedFsmState state)
{
    return state == LED_FSM_ON ? LED_FSM_ON : LED_FSM_OFF;
}

LedFsmState ledFsmGetInitialState()
{
    return LED_FSM_OFF;
}

LedFsmState ledFsmNextState(LedFsmState currentState, uint8_t input)
{
    LedFsmState state = normalizeState(currentState);
    uint8_t index = input ? 1 : 0;
    return fsmTable[state].next[index];
}

bool ledFsmOutputForState(LedFsmState state)
{
    return fsmTable[normalizeState(state)].ledOn;
}

const char* ledFsmStateName(LedFsmState state)
{
    return fsmTable[normalizeState(state)].name;
}

uint16_t ledFsmDelayMs(LedFsmState state)
{
    return fsmTable[normalizeState(state)].delayMs;
}
