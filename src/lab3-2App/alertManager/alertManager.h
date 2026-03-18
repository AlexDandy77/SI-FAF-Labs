#ifndef ALERT_MANAGER_H
#define ALERT_MANAGER_H

typedef enum {
    ALERT_NORMAL,     // Temperature within safe range
    ALERT_DEBOUNCE,   // Threshold crossed, confirming...
    ALERT_ACTIVE      // Alert confirmed (stable threshold breach)
} AlertState;

typedef struct {
    AlertState currentState;
    int        debounceCounter;
} AlertContext;

// Threshold configuration
#define TEMP_THRESHOLD    30.0f   // Base threshold (C)
#define TEMP_HYSTERESIS   1.0f    // Hysteresis band (C)
#define DEBOUNCE_COUNT    5       // Consecutive readings to confirm

// Derived thresholds (with hysteresis)
#define TEMP_UPPER  (TEMP_THRESHOLD + TEMP_HYSTERESIS)  // 31 C — trigger alert
#define TEMP_LOWER  (TEMP_THRESHOLD - TEMP_HYSTERESIS)  // 29 C — clear alert

void        alertInit(AlertContext *context);
AlertState  alertProcess(AlertContext *context, float value,
                         float upperThreshold, float lowerThreshold);
int         alertGetDebounceCounter(const AlertContext *context);
const char* alertStateToString(AlertState state);

#endif
