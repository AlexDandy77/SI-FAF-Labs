#include "taskManager.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#include "../actuatorControl/actuatorControl.h"
#include "../signalConditioning/signalConditioning.h"
#include "../reportDisplay/reportDisplay.h"
#include "../userInput/userInput.h"
#include "../sensorAcq/sensorAcq.h"
#include "../stateManager/stateManager.h"

void vTaskUserInput(void *pvParameters);
void vTaskConditioning(void *pvParameters);
void vTaskControl(void *pvParameters);
void vTaskDisplay(void *pvParameters);
void vTaskSensorAcq(void *pvParameters);

void taskManagerInit() {
    actuatorControlInit();
    reportDisplayInit();
    sensorAcqInit();

    xTaskCreate(vTaskUserInput,
                "UserInput",
                configMINIMAL_STACK_SIZE,
                NULL,
                3,  // High priority - fast UI response
                NULL);

    xTaskCreate(vTaskSensorAcq,
                "SensorAcq",
                configMINIMAL_STACK_SIZE,
                NULL,
                3,  // High priority - fast sensor response
                NULL);

    xTaskCreate(vTaskConditioning,
                "ConditionSignal",
                configMINIMAL_STACK_SIZE,
                NULL,
                2,  // Medium
                NULL);

    xTaskCreate(vTaskControl,
                "ActuatorControl",
                configMINIMAL_STACK_SIZE,
                NULL,
                2,  // Medium
                NULL);

    xTaskCreate(vTaskDisplay,
                "DisplayReport",
                configMINIMAL_STACK_SIZE + 128, // Formatting strings uses more stack
                NULL,
                1,  // Low priority
                NULL);
}

void vTaskSensorAcq(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(20);

    for (;;) {
        uint8_t state = sensorAcqReadBinaryState();
        // If sensor goes HIGH (1), it turns on the system.
        // We only set the raw command if it's explicitly HIGH, to avoid
        // continuously overwriting user serial input with LOW.
        if (state == 1) {
            setRawCommand('1');
        }
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void vTaskUserInput(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(20); // Polling serial 50ms

    for (;;) {
        userInputProcess();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void vTaskConditioning(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(20); // 50ms

    for (;;) {
        signalConditioningProcess();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void vTaskControl(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(20); // 100ms recurrence for actuator

    for (;;) {
        actuatorControlProcess();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void vTaskDisplay(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(3000); // 3000ms (3 seconds) for cleaner console

    for (;;) {
        reportDisplayProcess();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}