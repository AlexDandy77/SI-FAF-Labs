#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

// Task context structure (recurrence, offset, function pointer)
typedef struct {
    unsigned int recurrence;   // period in timer ticks (ms)
    unsigned int offset;       // initial delay in ticks
    void (*taskFunction)();    // pointer to task function
    volatile int counter;      // countdown to next execution
    volatile bool ready;       // flag set by ISR, cleared by scheduler
} TaskContext;

#define NUM_TASKS 3

extern TaskContext taskTable[NUM_TASKS];

void initTaskManager();
void runScheduler();

// Task functions
void taskButtonMonitor();    // Task 1: detect press/release, measure duration
void taskStatsBlink();       // Task 2: update stats, blink yellow LED
void taskPeriodicReport();   // Task 3: report every 10 s via STDIO

#endif