#include <Arduino.h>
#include "lab0App/lab0App.h"

#define APP_NAME LAB_0

void setup() {
  #if APP_NAME == LAB_0
    lab0AppSetup();
  #endif
}

void loop() {
  #if APP_NAME == LAB_0
    lab0AppLoop();
  #endif
}

