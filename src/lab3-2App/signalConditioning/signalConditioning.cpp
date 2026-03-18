#include "signalConditioning.h"

void signalConditioningInit(SignalConditioningContext *ctx, int minSat, int maxSat, float alpha) {
    if (!ctx) return;
    for (int i = 0; i < MEDIAN_WINDOW_SIZE; i++) {
        ctx->history[i] = 0;
    }
    ctx->historyIndex = 0;
    ctx->count = 0;
    ctx->emaValue = 0.0f;
    ctx->alpha = alpha;
    ctx->minSaturation = minSat;
    ctx->maxSaturation = maxSat;
}

static int getMedian(int *window, int count) {
    int temp[MEDIAN_WINDOW_SIZE];
    for (int i = 0; i < count; i++) {
        temp[i] = window[i];
    }
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (temp[j] > temp[j + 1]) {
                int t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }
    return temp[count / 2];
}

float signalConditioningProcess(SignalConditioningContext *ctx, int rawValue, int *rawOut, int *medianOut) {
    if (!ctx) return 0.0f;

    // 1. Saturation
    int saturatedValue = rawValue;
    if (saturatedValue < ctx->minSaturation) saturatedValue = ctx->minSaturation;
    if (saturatedValue > ctx->maxSaturation) saturatedValue = ctx->maxSaturation;
    
    if (rawOut) *rawOut = saturatedValue;

    // 2. Median Filter (Salt and Pepper)
    ctx->history[ctx->historyIndex] = saturatedValue;
    ctx->historyIndex = (ctx->historyIndex + 1) % MEDIAN_WINDOW_SIZE;
    if (ctx->count < MEDIAN_WINDOW_SIZE) {
        ctx->count++;
    }
    
    int median = getMedian(ctx->history, ctx->count);
    if (medianOut) *medianOut = median;

    // 3. Weighted Average (EMA)
    if (ctx->count == 1) {
        ctx->emaValue = (float)median;
    } else {
        ctx->emaValue = (ctx->alpha * (float)median) + ((1.0f - ctx->alpha) * ctx->emaValue);
    }

    return ctx->emaValue;
}
