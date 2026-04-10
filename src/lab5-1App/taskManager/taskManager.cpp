#include "taskManager.h"
#include <Arduino_FreeRTOS.h>

#include "../sensorAcq/sensorAcq.h"
#include "../userInput/userInput.h"
#include "../signalConditioning/signalConditioning.h"
#include "../actuatorControl/actuatorControl.h"
#include "../reportDisplay/reportDisplay.h"

#define TASK_SENSOR_STACK  (configMINIMAL_STACK_SIZE + 512)
#define TASK_USER_STACK    (configMINIMAL_STACK_SIZE + 64)
#define TASK_CONTROL_STACK (configMINIMAL_STACK_SIZE + 64)
#define TASK_REPORT_STACK  (configMINIMAL_STACK_SIZE + 256)

#define SENSOR_PERIOD_MS 2000
#define USER_PERIOD_MS   100
#define CONTROL_PERIOD_MS 100
#define REPORT_PERIOD_MS 500

void vTaskSensorAcq(void* pvParameters);
void vTaskUserInput(void* pvParameters);
void vTaskControl(void* pvParameters);
void vTaskReportDisplay(void* pvParameters);

void taskManagerInit() {
    sensorAcqInit();
    userInputInit();
    signalConditioningInit();
    actuatorControlInit();
    reportDisplayInit();

    xTaskCreate(vTaskSensorAcq, "Sensor", TASK_SENSOR_STACK, NULL, 2, NULL);
    xTaskCreate(vTaskUserInput, "User", TASK_USER_STACK, NULL, 2, NULL);
    xTaskCreate(vTaskControl, "Ctrl", TASK_CONTROL_STACK, NULL, 3, NULL);
    xTaskCreate(vTaskReportDisplay, "Rep", TASK_REPORT_STACK, NULL, 1, NULL);
}

void vTaskSensorAcq(void* pvParameters) {
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        sensorAcqProcess();
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(SENSOR_PERIOD_MS));
    }
}

void vTaskUserInput(void* pvParameters) {
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        userInputProcess();
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(USER_PERIOD_MS));
    }
}

void vTaskControl(void* pvParameters) {
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        signalConditioningProcess();
        actuatorControlProcess();
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(CONTROL_PERIOD_MS));
    }
}

void vTaskReportDisplay(void* pvParameters) {
    (void)pvParameters;
    vTaskDelay(pdMS_TO_TICKS(100)); // offset output
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        reportDisplayProcess();
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(REPORT_PERIOD_MS));
    }
}
