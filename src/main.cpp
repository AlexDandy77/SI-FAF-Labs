#include <Arduino.h>
#include "lab0App/lab0App.h"
#include "lab1App/lab1App.h"
#include "lab1-2App/lab1-2App.h"

#define LAB_0 0
#define LAB_1 1
#define LAB_2 2
#define APP_NAME LAB_2  // Change to select lab

void setup() {
  #if APP_NAME == LAB_0
    lab0AppSetup();
  #elif APP_NAME == LAB_1
    lab1AppSetup();
  #elif APP_NAME == LAB_2
    lab1_2AppSetup();
  #endif
}

void loop() {
  #if APP_NAME == LAB_0
    lab0AppLoop();
  #elif APP_NAME == LAB_1
    lab1AppLoop();
  #elif APP_NAME == LAB_2
    lab1_2AppLoop();
  #endif
}

