#include "taskManager.h"
#include "../sensorAcq/sensorAcq.h"
#include "../alertManager/alertManager.h"
#include "../stateManager/stateManager.h"
#include "../signalConditioning/signalConditioning.h"
#include "../../ddLed/ddLed.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <semphr.h>
#include <stdio.h>

// Pin assignments
#define TEMP_SENSOR_PIN       A0
#define LIGHT_SENSOR_DO_PIN   7

#define LED_GREEN             13    // All sensors in NORMAL
#define LED_RED               12    // Any sensor in ALERT
#define LED_YELLOW            11    // Any sensor in DEBOUNCE

// Light digital channel thresholds (0.0 = light, 1.0 = dark)
#define LIGHT_THRESHOLD        0.5f
#define LIGHT_HYSTERESIS       0.3f
#define LIGHT_UPPER   (LIGHT_THRESHOLD + LIGHT_HYSTERESIS)
#define LIGHT_LOWER   (LIGHT_THRESHOLD - LIGHT_HYSTERESIS)

// LCD (I2C, 16x2, address 0x27)
static LiquidCrystal_I2C lcd(0x27, 16, 2);
static AlertContext      tempAlertContext;
static AlertContext      lightAlertContext;
static SignalConditioningContext tempConditioningContext;

static const char *alertStateShort(AlertState state)
{
    switch (state) {
        case ALERT_NORMAL:   return "NRM";
        case ALERT_DEBOUNCE: return "DBN";
        case ALERT_ACTIVE:   return "ALR";
        default:             return "UNK";
    }
}

static const char *lightLevelToString(int lightDigital)
{
    return lightDigital ? "DARK" : "LIGHT";
}

static void updateSystemLeds(AlertState tempState, AlertState lightState)
{
    int anyAlert = (tempState == ALERT_ACTIVE) || (lightState == ALERT_ACTIVE);
    int anyDebounce = (tempState == ALERT_DEBOUNCE) || (lightState == ALERT_DEBOUNCE);

    if (anyAlert) {
        ddLedOff(LED_GREEN);
        ddLedOn(LED_RED);
        ddLedOff(LED_YELLOW);
        return;
    }

    if (anyDebounce) {
        ddLedOff(LED_GREEN);
        ddLedOff(LED_RED);
        ddLedToggle(LED_YELLOW);
        return;
    }

    ddLedOn(LED_GREEN);
    ddLedOff(LED_RED);
    ddLedOff(LED_YELLOW);
}

// ---- Initialization: hardware + LCD + FreeRTOS tasks ----
void initTaskManager()
{
    sensorInit(TEMP_SENSOR_PIN);
    pinMode(LIGHT_SENSOR_DO_PIN, INPUT);

    ddLedInit(LED_GREEN);
    ddLedInit(LED_RED);
    ddLedInit(LED_YELLOW);
    alertInit(&tempAlertContext);
    alertInit(&lightAlertContext);
    
    // Init with min=100, max=900 limits for the analog sensor, alpha=0.2 (weighted avg)
    signalConditioningInit(&tempConditioningContext, 100, 900, 0.2f);

    // Initialize LCD
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Lab3-2 Analog");
    lcd.setCursor(0, 1);
    lcd.print("Initializing...");

    xTaskCreate(taskSensorAcquisition, "SnsAcq", TASK_ACQ_STACK,
                NULL, TASK_ACQ_PRIORITY, NULL);
    xTaskCreate(taskAlertConditioning, "Alert",  TASK_ALERT_STACK,
                NULL, TASK_ALERT_PRIORITY, NULL);
    xTaskCreate(taskDisplayReport,     "Displ",  TASK_DISPLAY_STACK,
                NULL, TASK_DISPLAY_PRIORITY, NULL);
}

// ---- Task 1: Dual Sensor Acquisition (50 ms, priority 3) ----
// Reads temperature from NTC (analog) and light state from LDR DO (digital),
// then stores both channels in shared state under mutex.
void taskSensorAcquisition(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        int   raw  = sensorReadRaw(TEMP_SENSOR_PIN);
        int   rawSat = raw, rawMed = raw;
        float rawEma = signalConditioningProcess(&tempConditioningContext, raw, &rawSat, &rawMed);
        float temp = sensorRawToTemperature((int)rawEma);
        
        int   lightDigital = (digitalRead(LIGHT_SENSOR_DO_PIN) == HIGH) ? 1 : 0;

        if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
            sensorState.tempRawAdc    = raw;
            sensorState.tempSaturated = rawSat;
            sensorState.tempMedian    = rawMed;
            sensorState.tempEmaRaw    = rawEma;
            sensorState.temperature   = temp;
            sensorState.lightDigital  = lightDigital;
            xSemaphoreGive(stateMutex);
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TASK_ACQ_PERIOD_MS));
    }
}

// ---- Task 2: Dual Alert Conditioning (100 ms, priority 2) ----
// Processes independent hysteresis/debounce for temperature and light, then
// updates shared state and global LED indication.
void taskAlertConditioning(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        float temp = 0.0f;
        int   lightDigital = 0;

        if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
            temp         = sensorState.temperature;
            lightDigital = sensorState.lightDigital;
            xSemaphoreGive(stateMutex);
        }

        AlertState tempState = alertProcess(&tempAlertContext, temp,
                                            TEMP_UPPER, TEMP_LOWER);

        float lightValue = lightDigital ? 1.0f : 0.0f;
        AlertState lightState = alertProcess(&lightAlertContext, lightValue,
                                             LIGHT_UPPER, LIGHT_LOWER);

        int tempDbCount  = alertGetDebounceCounter(&tempAlertContext);
        int lightDbCount = alertGetDebounceCounter(&lightAlertContext);

        if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
            sensorState.tempAlertState       = tempState;
            sensorState.tempDebounceCounter  = tempDbCount;
            sensorState.lightAlertState      = lightState;
            sensorState.lightDebounceCounter = lightDbCount;
            xSemaphoreGive(stateMutex);
        }

        updateSystemLeds(tempState, lightState);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TASK_ALERT_PERIOD_MS));
    }
}

// ---- Task 3: Display & Reporting (500 ms, priority 1) ----
// Snapshots shared state under mutex, prints report to Serial
// via printf(), updates LCD with compact status.
void taskDisplayReport(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TASK_DISPLAY_PERIOD_MS));

        // Snapshot state under mutex
        int        tempRaw = 0;
        int        tempSaturated = 0;
        int        tempMedian = 0;
        float      tempEmaRaw = 0.0f;
        float      temp = 0.0f;
        AlertState tempState = ALERT_NORMAL;

        int        lightDigital = 0;
        AlertState lightState = ALERT_NORMAL;

        if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
            tempRaw       = sensorState.tempRawAdc;
            tempSaturated = sensorState.tempSaturated;
            tempMedian    = sensorState.tempMedian;
            tempEmaRaw    = sensorState.tempEmaRaw;
            temp          = sensorState.temperature;
            tempState     = sensorState.tempAlertState;

            lightDigital  = sensorState.lightDigital;
            lightState    = sensorState.lightAlertState;
            xSemaphoreGive(stateMutex);
        }

        const char *tempStateStr  = alertStateToString(tempState);
        const char *lightStateStr = alertStateToString(lightState);
        const char *lightLevelStr = lightLevelToString(lightDigital);

        // ---- Serial report (printf via srvSerialStdio) ----
        int tempTenths = (int)(temp * 10.0f);
        int tWhole     = tempTenths / 10;
        int tFrac      = abs(tempTenths % 10);

        printf("--- Analog & Digital Monitor ---\r\n");
        printf("Temp Raw ADC:    %d\r\n", tempRaw);
        printf("Temp Saturated:  %d [100..900]\r\n", tempSaturated);
        printf("Temp Median (5): %d\r\n", tempMedian);
        printf("Temp EMA (a=0.2):%d\r\n", (int)tempEmaRaw);
        printf("Temperature:     %d.%d C\r\n", tWhole, tFrac);
        printf("Temp state:      %s\r\n", tempStateStr);

        printf("Light DO:        %s\r\n", lightLevelStr);
        printf("Light state:     %s\r\n", lightStateStr);
        printf("--------------------------------\r\n\r\n");

        // ---- LCD display (16x2) ----
        char line0[17];
        char line1[17];

        snprintf(line0, sizeof(line0), "T:%d.%dC %-3s",
                 tWhole, tFrac, alertStateShort(tempState));
        snprintf(line1, sizeof(line1), "L:%s %-3s",
                 lightLevelStr, alertStateShort(lightState));

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(line0);

        lcd.setCursor(0, 1);
        lcd.print(line1);
    }
}
