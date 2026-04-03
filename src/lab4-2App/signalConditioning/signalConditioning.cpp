#include "signalConditioning.h"
#include "../stateManager/stateManager.h"
#include <Arduino.h>
#include <string.h>

// ---- Binary debounce ----
#define BINARY_DEBOUNCE_THRESHOLD 2

static char lastRawBinary   = '0';
static int  binaryDebounce  = 0;

// ---- Analog signal conditioning ----
#define MEDIAN_SIZE  5
#define RAMP_STEP    5    // max units per cycle
#define OVERLOAD_THR 230  // roughly ~90% of 255

static int  medianBuf[MEDIAN_SIZE] = {0};
static int  medianIdx = 0;
static int  prevConditioned = 0;

static int median5(int *buf)
{
    int tmp[MEDIAN_SIZE];
    memcpy(tmp, buf, sizeof(tmp));
    // Insertion sort (5 elements, no heap needed)
    for (int i = 1; i < MEDIAN_SIZE; i++) {
        int key = tmp[i];
        int j = i - 1;
        while (j >= 0 && tmp[j] > key) {
            tmp[j + 1] = tmp[j];
            j--;
        }
        tmp[j + 1] = key;
    }
    return tmp[MEDIAN_SIZE / 2];
}

void signalConditioningProcess()
{
    // --- Binary debounce ---
    char raw     = getRawBinaryCmd();
    char current = getValidatedBinaryCmd();

    if (raw != current) {
        if (raw == lastRawBinary) {
            binaryDebounce++;
            if (binaryDebounce >= BINARY_DEBOUNCE_THRESHOLD) {
                setValidatedBinaryCmd(raw);
                binaryDebounce = 0;
            }
        } else {
            binaryDebounce  = 1;
            lastRawBinary   = raw;
        }
    } else {
        binaryDebounce = 0;
        lastRawBinary  = raw;
    }

    // --- Analog conditioning ---

    // 1. Saturation: clamp raw input to [0, 255]
    int rawVal = getRawAnalogTarget();
    if (rawVal < 0)   rawVal = 0;
    if (rawVal > 255) rawVal = 255;

    // 2. Median filter: accumulate samples, pick middle value
    medianBuf[medianIdx % MEDIAN_SIZE] = rawVal;
    medianIdx++;
    int med = median5(medianBuf);

    // 3. Weighted average: 30% new + 70% previous (reduces fluctuations)
    int weighted = (int)(0.3f * (float)med + 0.7f * (float)prevConditioned);
    prevConditioned = weighted;
    setConditionedTarget(weighted);

    // 4. Ramp: move rampedSpeed toward conditioned target by max RAMP_STEP/cycle
    int ramped = getRampedSpeed();
    int diff   = weighted - ramped;
    if (diff > RAMP_STEP)        ramped += RAMP_STEP;
    else if (diff < -RAMP_STEP)  ramped -= RAMP_STEP;
    else                          ramped  = weighted;

    setRampedSpeed(ramped);

    // 5. Overload alert
    setAnalogOverload(ramped > OVERLOAD_THR);
}
