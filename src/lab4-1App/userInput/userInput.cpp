#include "userInput.h"
#include <Arduino.h>
#include <stdio.h>
#include "../../srvSerialStdio/srvSerialStdio.h"
#include "../stateManager/stateManager.h"

void userInputProcess() {
    if (srvSerialAvailable() > 0) {
        int c = getchar();
        if (c == '1' || c == 'O' || c == 'o') {
            setRawCommand('1'); // ON
        } else if (c == '0' || c == 'F' || c == 'f') {
            setRawCommand('0'); // OFF
        }
    }
}