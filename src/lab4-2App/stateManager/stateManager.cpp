#include "stateManager.h"
#include <Arduino.h>

static SemaphoreHandle_t stateMutex = NULL;

// Binary actuator state
static char rawBinaryCmd      = '0';
static char validatedBinaryCmd = '0';
static bool binaryActuatorOn  = false;

// Analog actuator state
static int  rawAnalogTarget   = 0;
static int  conditionedTarget = 0;
static int  rampedSpeed       = 0;
static bool analogOverload    = false;

void stateManagerInit()
{
    stateMutex = xSemaphoreCreateMutex();
}

// ----- Binary actuator -----

void setRawBinaryCmd(char cmd)
{
    if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        rawBinaryCmd = cmd;
        xSemaphoreGive(stateMutex);
    }
}

char getRawBinaryCmd()
{
    char v = '0';
    if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        v = rawBinaryCmd;
        xSemaphoreGive(stateMutex);
    }
    return v;
}

void setValidatedBinaryCmd(char cmd)
{
    if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        validatedBinaryCmd = cmd;
        xSemaphoreGive(stateMutex);
    }
}

char getValidatedBinaryCmd()
{
    char v = '0';
    if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        v = validatedBinaryCmd;
        xSemaphoreGive(stateMutex);
    }
    return v;
}

void setBinaryActuatorOn(bool on)
{
    if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        binaryActuatorOn = on;
        xSemaphoreGive(stateMutex);
    }
}

bool getBinaryActuatorOn()
{
    bool v = false;
    if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        v = binaryActuatorOn;
        xSemaphoreGive(stateMutex);
    }
    return v;
}

// ----- Analog actuator -----

void setRawAnalogTarget(int val)
{
    if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        rawAnalogTarget = val;
        xSemaphoreGive(stateMutex);
    }
}

int getRawAnalogTarget()
{
    int v = 0;
    if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        v = rawAnalogTarget;
        xSemaphoreGive(stateMutex);
    }
    return v;
}

void setConditionedTarget(int val)
{
    if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        conditionedTarget = val;
        xSemaphoreGive(stateMutex);
    }
}

int getConditionedTarget()
{
    int v = 0;
    if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        v = conditionedTarget;
        xSemaphoreGive(stateMutex);
    }
    return v;
}

void setRampedSpeed(int val)
{
    if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        rampedSpeed = val;
        xSemaphoreGive(stateMutex);
    }
}

int getRampedSpeed()
{
    int v = 0;
    if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        v = rampedSpeed;
        xSemaphoreGive(stateMutex);
    }
    return v;
}

void setAnalogOverload(bool flag)
{
    if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        analogOverload = flag;
        xSemaphoreGive(stateMutex);
    }
}

bool getAnalogOverload()
{
    bool v = false;
    if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        v = analogOverload;
        xSemaphoreGive(stateMutex);
    }
    return v;
}

// Aliases for naming convention used in report/actuator layers
bool actuator_get_state() { return getBinaryActuatorOn(); }
int  actuator_get_speed() { return getRampedSpeed(); }
