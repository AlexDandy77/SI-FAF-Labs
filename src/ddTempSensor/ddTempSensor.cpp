#include "ddTempSensor.h"
#include <DHT.h>

static DHT dht(0, DHT22);
static bool isInit = false;

void ddTempSensorInit(int pin) {
    if (!isInit) {
        dht = DHT(pin, DHT22);
        dht.begin();
        isInit = true;
    }
}

float ddTempSensorRead() {
    if (!isInit) return 0.0f;
    float t = dht.readTemperature();
    if (isnan(t)) return -999.0f; // error value
    return t;
}
