#ifndef LAB4_2_USER_INPUT_H
#define LAB4_2_USER_INPUT_H

void userInputSerialProcess();   // Non-blocking serial poll for binary command
void userInputKeypadTask();      // Blocking keypad accumulator loop (runs as task body)

#endif
