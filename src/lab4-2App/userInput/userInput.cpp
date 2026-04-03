#include "userInput.h"
#include <Arduino.h>
#include <stdio.h>
#include "../../srvSerialStdio/srvSerialStdio.h"
#include "../stateManager/stateManager.h"

// Called periodically from vTaskSerialInput (non-blocking).
void userInputSerialProcess()
{
    while (srvSerialAvailable() > 0) {
        char c = (char)srvSerialReadByte(NULL);
        if (c == '1' || c == 'O' || c == 'o') {
            setRawBinaryCmd('1');
        } else if (c == '0' || c == 'F' || c == 'f') {
            setRawBinaryCmd('0');
        }
    }
}

// Runs as the body of vTaskKeypadInput.
// Blocks on getchar() which reads from keypadInput (stdin).
// FreeRTOS-safe because ddKeypadGetChar uses delay() which maps to vTaskDelay().
void userInputKeypadTask()
{
    char buf[4];
    int  idx = 0;

    for (;;) {
        char key = (char)getchar();

        if (key >= '0' && key <= '9') {
            if (idx < 3) {
                buf[idx++] = key;
            }
            // Silently ignore more than 3 digits; saturation handles >255 anyway.
        } else if (key == '#') {
            if (idx > 0) {
                buf[idx] = '\0';
                int val = atoi(buf);
                setRawAnalogTarget(val);
                idx = 0;
            }
        } else if (key == '*') {
            idx = 0;
        }
        // All other keys (A–D) are ignored.
    }
}
