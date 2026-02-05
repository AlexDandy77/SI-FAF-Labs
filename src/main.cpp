#include <Arduino.h>
#include "lab0App/lab0App.h"
#include "lab1App/lab1App.h"

#define LAB_0 0
#define LAB_1 1
#define APP_NAME LAB_1  // Change to select lab

void setup() {
  #if APP_NAME == LAB_0
    lab0AppSetup();
  #elif APP_NAME == LAB_1
    lab1AppSetup();
  #endif
}

void loop() {
  #if APP_NAME == LAB_0
    lab0AppLoop();
  #elif APP_NAME == LAB_1
    lab1AppLoop();
  #endif
}

