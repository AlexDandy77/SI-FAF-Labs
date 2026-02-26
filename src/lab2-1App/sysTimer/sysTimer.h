#ifndef SYS_TIMER_H
#define SYS_TIMER_H

#include <Arduino.h>

// 1 ms system tick (Timer1 CTC at 16 MHz / 64 / 250)
#define SYS_TICK_MS 1

void initSystemTimer();

#endif