#include "ddKeypad/ddKeypad.h"
#include "ddLcd/ddLcd.h"
#include "ddLed/ddLed.h"

#define LED_RED_PIN 13
#define LED_GREEN_PIN 12
#define MAX_CODE_LENGTH 4
#define VALID_CODE "708#"

static char enteredCode[MAX_CODE_LENGTH + 1];
static int codeIndex = 0;

static void setStatusLeds(bool isValid)
{
    if (isValid) {
        ddLedOn(LED_GREEN_PIN);
        ddLedOff(LED_RED_PIN);
    } else {
        ddLedOff(LED_GREEN_PIN);
        ddLedOn(LED_RED_PIN);
    }
}

void passcodeInit()
{
    codeIndex = 0;
    memset(enteredCode, 0, sizeof(enteredCode));
    printf("\fEnter code:\n");
}

static void validatePasscode()
{
    bool isValid = (strcmp(enteredCode, VALID_CODE) == 0);
    printf(isValid ? "\fCode Valid!" : "\fInvalid Code!");
    setStatusLeds(isValid);

    delay(2000);

    passcodeInit();
    ddLedOff(LED_GREEN_PIN);
    ddLedOff(LED_RED_PIN);
}

void lab1_2AppSetup()
{
    ddLedInit(LED_RED_PIN);
    ddLedInit(LED_GREEN_PIN);
    ddLcdInit();
    ddKeypadInit();
    passcodeInit();
}

void lab1_2AppLoop()
{
    if (codeIndex < MAX_CODE_LENGTH) {
        char key = 0;
        scanf("%c", &key);
        enteredCode[codeIndex++] = key;
        printf("%c", key);

        if (codeIndex == MAX_CODE_LENGTH) {
            // add null terminator to the end of the string
            enteredCode[codeIndex] = '\0';
            delay(500);
            validatePasscode();
        }
    }
}

