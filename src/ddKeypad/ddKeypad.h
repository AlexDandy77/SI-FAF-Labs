#ifndef KEYPAD_CONTROL_H
#define KEYPAD_CONTROL_H

#include <Arduino.h>
#include <Keypad.h>
#include <stdio.h>

extern FILE keypadInput;

#define ROWS 4
#define COLS 4

const char KEYS[ROWS][COLS] = {
    { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' }
};

const byte ROW_PINS[ROWS] = { 11, 10, 9, 8 };
const byte COL_PINS[COLS] = { 7, 6, 5, 4 };

void ddKeypadInit();
int ddKeypadGetChar(FILE* stream);

#endif