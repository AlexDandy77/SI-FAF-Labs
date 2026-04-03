#include "ddKeypad.h"
#include <Arduino_FreeRTOS.h>

static Keypad keypad(makeKeymap((char*)KEYS), (byte*)ROW_PINS, (byte*)COL_PINS, ROWS, COLS);
FILE keypadInput = { 0 };

void ddKeypadInit()
{
    fdev_setup_stream(&keypadInput, NULL, ddKeypadGetChar, _FDEV_SETUP_READ);
    stdin = &keypadInput;
}

int ddKeypadGetChar(FILE* stream)
{
    for (;;) {
        char key = keypad.getKey();
        if (key != NO_KEY) {
            return key;
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}