#include <Arduino.h>
#include "lab0App/lab0App.h"
#include "lab1App/lab1App.h"
#include "lab1-2App/lab1-2App.h"
#include "lab4-1App/lab4-1App.h"

#define LAB_0   0
#define LAB_1   1
#define LAB_1_2 2
#define LAB_2_1 3
#define LAB_2_2 4
#define LAB_3_1 5
#define LAB_3_2 6
#define LAB_4_1 7
#define LAB_4_2 8

#ifndef APP_NAME
  #define APP_NAME LAB_2_1  // Default; overridden by build_flags per env
#endif

#if APP_NAME == LAB_2_1
  #include "lab2-1App/lab2-1App.h"
#elif APP_NAME == LAB_2_2
  #include "lab2-2App/lab2-2App.h"
#elif APP_NAME == LAB_3_1
  #include "lab3-1App/lab3-1App.h"
#elif APP_NAME == LAB_3_2
  #include "lab3-2App/lab3-2App.h"
#elif APP_NAME == LAB_4_2
  #include "lab4-2App/lab4-2App.h"
#endif

void setup() {
  #if APP_NAME == LAB_0
    lab0AppSetup();
  #elif APP_NAME == LAB_1
    lab1AppSetup();
  #elif APP_NAME == LAB_1_2
    lab1_2AppSetup();
  #elif APP_NAME == LAB_2_1
    lab2_1AppSetup();
  #elif APP_NAME == LAB_2_2
    lab2_2AppSetup();
  #elif APP_NAME == LAB_3_1
    lab3_1AppSetup();
  #elif APP_NAME == LAB_3_2
    lab3_2AppSetup();
  #elif APP_NAME == LAB_4_1
    lab4_1AppSetup();
  #elif APP_NAME == LAB_4_2
    lab4_2AppSetup();
  #endif
}

void loop() {
  #if APP_NAME == LAB_0
    lab0AppLoop();
  #elif APP_NAME == LAB_1
    lab1AppLoop();
  #elif APP_NAME == LAB_1_2
    lab1_2AppLoop();
  #elif APP_NAME == LAB_2_1
    lab2_1AppLoop();
  #elif APP_NAME == LAB_2_2
    lab2_2AppLoop();
  #elif APP_NAME == LAB_3_1
    lab3_1AppLoop();
  #elif APP_NAME == LAB_3_2
    lab3_2AppLoop();
  #elif APP_NAME == LAB_4_1
    lab4_1AppLoop();
  #elif APP_NAME == LAB_4_2
    lab4_2AppLoop();
  #endif
}

