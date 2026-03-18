#ifndef SIGNAL_CONDITIONING_H
#define SIGNAL_CONDITIONING_H

#define MEDIAN_WINDOW_SIZE 5

typedef struct {
    int history[MEDIAN_WINDOW_SIZE];
    int historyIndex;
    int count;
    float emaValue;
    float alpha;
    int minSaturation;
    int maxSaturation;
} SignalConditioningContext;

void signalConditioningInit(SignalConditioningContext *ctx, int minSat, int maxSat, float alpha);

// Processes a raw value and returns the smoothed float result.
// Also outputs intermediate mapped raw value and median value.
float signalConditioningProcess(SignalConditioningContext *ctx, int rawValue, int *rawOut, int *medianOut);

#endif
