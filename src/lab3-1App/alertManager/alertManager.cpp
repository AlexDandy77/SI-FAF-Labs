#include "alertManager.h"

void alertInit(AlertContext *context)
{
    if (context == 0) {
        return;
    }

    context->currentState    = ALERT_NORMAL;
    context->debounceCounter = 0;
}

AlertState alertProcess(AlertContext *context, float value,
                        float upperThreshold, float lowerThreshold)
{
    if (context == 0) {
        return ALERT_NORMAL;
    }

    switch (context->currentState) {
        case ALERT_NORMAL:
            if (value >= upperThreshold) {
                context->debounceCounter++;
                if (context->debounceCounter >= DEBOUNCE_COUNT) {
                    context->currentState = ALERT_ACTIVE;
                    context->debounceCounter = 0;
                } else {
                    context->currentState = ALERT_DEBOUNCE;
                }
            } else {
                context->debounceCounter = 0;
            }
            break;

        case ALERT_DEBOUNCE:
            if (value >= upperThreshold) {
                context->debounceCounter++;
                if (context->debounceCounter >= DEBOUNCE_COUNT) {
                    context->currentState = ALERT_ACTIVE;
                    context->debounceCounter = 0;
                }
            } else if (value <= lowerThreshold) {
                // Dropped below lower threshold — cancel debounce
                context->currentState    = ALERT_NORMAL;
                context->debounceCounter = 0;
            }
            // Between LOWER and UPPER: stay in DEBOUNCE, keep counter
            break;

        case ALERT_ACTIVE:
            if (value <= lowerThreshold) {
                // Hysteresis: clear only when below lower threshold
                context->currentState    = ALERT_NORMAL;
                context->debounceCounter = 0;
            }
            break;
    }

    return context->currentState;
}

int alertGetDebounceCounter(const AlertContext *context)
{
    if (context == 0) {
        return 0;
    }

    return context->debounceCounter;
}

const char* alertStateToString(AlertState state)
{
    switch (state) {
        case ALERT_NORMAL:   return "NORMAL";
        case ALERT_DEBOUNCE: return "DEBOUNCE";
        case ALERT_ACTIVE:   return "ALERT";
        default:             return "UNKNOWN";
    }
}
