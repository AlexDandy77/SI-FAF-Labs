#include "sensorAcq.h"
#include "../../ddTempSensor/ddTempSensor.h"
#include "../stateManager/stateManager.h"

#define DHT_PIN 7

void sensorAcqInit() {
    ddTempSensorInit(DHT_PIN);
}

void sensorAcqProcess() {
    float t = ddTempSensorRead();
    if (t != -999.0f) {
        updateCurrentTemp(t);
        updateTempValid(true);
    } else {
        updateTempValid(false);
    }
}
