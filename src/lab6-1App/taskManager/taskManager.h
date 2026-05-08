#ifndef LAB6_1_TASK_MANAGER_H
#define LAB6_1_TASK_MANAGER_H

#include <Arduino_FreeRTOS.h>

#define TASK_FSM_STACK_WORDS    (configMINIMAL_STACK_SIZE + 96)
#define TASK_REPORT_STACK_WORDS (configMINIMAL_STACK_SIZE + 192)

#define TASK_FSM_PRIORITY    2
#define TASK_REPORT_PRIORITY 1

bool taskManagerInit();
void taskFsmController(void* pvParameters);

#endif
