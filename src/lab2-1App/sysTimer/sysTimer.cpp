#include "sysTimer.h"
#include "../taskManager/taskManager.h"

void initSystemTimer()
{
    cli(); // disable interrupts

    // reset Timer1 registers
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    // 1 ms tick: 16 MHz / 64 / 250 = 1000 Hz
    OCR1A = 249;

    // CTC mode (clear timer on compare match)
    TCCR1B |= (1 << WGM12);

    // prescaler 64: CS11 + CS10
    TCCR1B |= (1 << CS11) | (1 << CS10);

    // enable Timer1 compare-match interrupt
    TIMSK1 |= (1 << OCIE1A);

    sei(); // enable interrupts
}

// ISR fires every 1 ms — only sets ready flags, never calls tasks
ISR(TIMER1_COMPA_vect)
{
    for (int i = 0; i < NUM_TASKS; i++) {
        if (--taskTable[i].counter <= 0) {
            taskTable[i].counter = taskTable[i].recurrence;
            taskTable[i].ready = true;
        }
    }
}