#include "taskManager.h"
#include "../actuatorControl/actuatorControl.h"
#include "../buttonInput/buttonInput.h"
#include "../fsm/fsm.h"
#include "../reportDisplay/reportDisplay.h"
#include "../stateManager/stateManager.h"
#include <stdio.h>

#define FSM_PERIOD_MS 10

static bool createTask(TaskFunction_t task,
                       const char* name,
                       uint16_t stackWords,
                       UBaseType_t priority)
{
    BaseType_t result = xTaskCreate(task,
                                    name,
                                    stackWords,
                                    NULL,
                                    priority,
                                    NULL);

    if (result != pdPASS) {
        printf("ERROR: failed to create task %s\r\n", name);
        return false;
    }

    printf("Task ready: %s\r\n", name);
    return true;
}

bool taskManagerInit()
{
    buttonInputInit();
    actuatorControlInit();
    reportDisplayInit();

    bool ok = true;
    ok = createTask(taskFsmController, "FSM", TASK_FSM_STACK_WORDS, TASK_FSM_PRIORITY) && ok;
    ok = createTask(taskReportDisplay, "Report", TASK_REPORT_STACK_WORDS, TASK_REPORT_PRIORITY) && ok;
    return ok;
}

void taskFsmController(void* pvParameters)
{
    (void)pvParameters;
    TickType_t lastWake = xTaskGetTickCount();

    for (;;) {
        if (buttonInputPollPressEvent()) {
            Lab61Snapshot snapshot;
            stateManagerGetSnapshot(&snapshot);

            LedFsmState nextState = ledFsmNextState(snapshot.state, 1);
            stateManagerSetFsmState(nextState, true);
            actuatorControlProcess();
            reportDisplayPrintCurrentState();
        }

        actuatorControlProcess();
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(FSM_PERIOD_MS));
    }
}
