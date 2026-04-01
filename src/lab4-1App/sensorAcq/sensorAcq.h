#ifndef SENSOR_ACQ_H
#define SENSOR_ACQ_H

#include <stdint.h>

// Initializes the sensor driver logic
void sensorAcqInit();

// Reads the raw binary sensor state (0 or 1)
uint8_t sensorAcqReadBinaryState();

#endif // SENSOR_ACQ_H
