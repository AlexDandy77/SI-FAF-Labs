#include "taskManager.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#include "../actuatorControl/actuatorControl.h"
#include "../signalConditioning/signalConditioning.h"
#include "../reportDisplay/reportDisplay.h"
#include "../userInput/userInput.h"
#include "../stateManager/stateManager.h"

static void vTaskSerialInput(void *pvParameters);
static void vTaskKeypadInput(void *pvParameters);
static void vTaskSignalCond(void *pvParameters);
static void vTaskActuatorCtrl(void *pvParameters);
static void vTaskDisplay(void *pvParameters);

void taskManagerInit()
{
    actuatorControlInit();
    reportDisplayInit();

    // Priority 3 — fast serial poll for binary relay command
    xTaskCreate(vTaskSerialInput,
                "SerialIn",
                configMINIMAL_STACK_SIZE,
                NULL,
                3,
                NULL);

    // Priority 2 — blocking keypad accumulator for analog speed entry
    xTaskCreate(vTaskKeypadInput,
                "KeypadIn",
                configMINIMAL_STACK_SIZE + 64,
                NULL,
                2,
                NULL);

    // Priority 2 — debounce binary + 4-stage analog conditioning
    xTaskCreate(vTaskSignalCond,
                "SigCond",
                configMINIMAL_STACK_SIZE,
                NULL,
                2,
                NULL);

    // Priority 2 — drive relay and PWM output
    xTaskCreate(vTaskActuatorCtrl,
                "ActCtrl",
                configMINIMAL_STACK_SIZE,
                NULL,
                2,
                NULL);

    // Priority 1 — serial printf + LCD update every 500 ms
    xTaskCreate(vTaskDisplay,
                "Display",
                configMINIMAL_STACK_SIZE + 256,
                NULL,
                1,
                NULL);
}

static void vTaskSerialInput(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        userInputSerialProcess();
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(50));
    }
}

static void vTaskKeypadInput(void *pvParameters)
{
    // Runs the blocking keypad FSM loop; never returns.
    userInputKeypadTask();
}

static void vTaskSignalCond(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        signalConditioningProcess();
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(50));
    }
}

static void vTaskActuatorCtrl(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        actuatorControlProcess();
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(50));
    }
}

static void vTaskDisplay(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        reportDisplayProcess();
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(500));
    }
}
