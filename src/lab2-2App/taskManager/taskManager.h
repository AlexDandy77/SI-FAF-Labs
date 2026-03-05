#ifndef TASK_MANAGER_2_2_H
#define TASK_MANAGER_2_2_H

#include <Arduino_FreeRTOS.h>

// ---- Task configuration (mirrors lab2-1 table) ----
// Task 1: Button monitor   — period 20 ms,  priority 3 (highest)
// Task 2: Stats/blink       — event-driven,  priority 2
// Task 3: Periodic report   — period 10 s,   priority 1 (lowest)

#define TASK1_PERIOD_MS   20
#define TASK2_PERIOD_MS   100     // blink toggle interval (same as lab2-1 recurrence)
#define TASK3_PERIOD_MS   10000

#define TASK1_STACK_WORDS 100
#define TASK2_STACK_WORDS 100
#define TASK3_STACK_WORDS 164

#define TASK1_PRIORITY    3
#define TASK2_PRIORITY    2
#define TASK3_PRIORITY    1

void initTaskManager();

// FreeRTOS task functions (pvParameters unused)
void taskButtonMonitor(void *pvParameters);
void taskStatsBlink(void *pvParameters);
void taskPeriodicReport(void *pvParameters);

#endif
