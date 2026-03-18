#ifndef TASK_MANAGER_3_2_H
#define TASK_MANAGER_3_2_H

#include <Arduino_FreeRTOS.h>

// ---- Task configuration ----
// Task 1: Dual sensor acquisition  — period 50 ms,  priority 3 (highest)
// Task 2: Dual alert conditioning  — period 100 ms, priority 2
// Task 3: Display / reporting      — period 500 ms, priority 1 (lowest)

#define TASK_ACQ_PERIOD_MS       50
#define TASK_ALERT_PERIOD_MS     100
#define TASK_DISPLAY_PERIOD_MS   500

// Stack sizes (words; 1 word = 2 bytes on AVR)
#define TASK_ACQ_STACK           150
#define TASK_ALERT_STACK         128
#define TASK_DISPLAY_STACK       256

// Priorities
#define TASK_ACQ_PRIORITY        3
#define TASK_ALERT_PRIORITY      2
#define TASK_DISPLAY_PRIORITY    1

void initTaskManager();

void taskSensorAcquisition(void *pvParameters);
void taskAlertConditioning(void *pvParameters);
void taskDisplayReport(void *pvParameters);

#endif
