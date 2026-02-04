#include <Arduino.h>
#include "ddButton/ddButton.h"
#include "ddLed/ddLed.h"

#define LED_PIN 13
#define BUTTON_PIN 12

void lab0AppSetup() {
  ddLedSetup(LED_PIN);
  ddButtonSetup(BUTTON_PIN);
}

void lab0AppLoop() {
  // Wait UNTIL button is pressed
  while(!ddButtonIsPressed(BUTTON_PIN));
  
  // Debounce delay - wait for button to stabilize
  delay(50);
  
  // Verify button is still pressed after debounce
  if(ddButtonIsPressed(BUTTON_PIN)) {
    ddLedToggle(LED_PIN);
    
    // Wait UNTIL button is released
    while(ddButtonIsPressed(BUTTON_PIN));
    
    // Debounce delay for release
    delay(50);
  }
}

