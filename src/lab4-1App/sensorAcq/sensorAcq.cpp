#include "sensorAcq.h"
#include <Arduino.h>

// Define a sensor pin, e.g., a simple button or typical LDR DO hookup
#define BINARY_SENSOR_PIN 7

void sensorAcqInit() {
    pinMode(BINARY_SENSOR_PIN, INPUT_PULLUP);
}

uint8_t sensorAcqReadBinaryState() {
    // Return 1 if pressed/active (LOW due to INPUT_PULLUP)
    return digitalRead(BINARY_SENSOR_PIN) == LOW ? 1 : 0;
}
