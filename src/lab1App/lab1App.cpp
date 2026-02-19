#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lab1App.h"
#include "ddLed/ddLed.h"
#include "srvSerialStdio/srvSerialStdio.h"

#define LED_PIN 13
#define BAUD_RATE 9600
#define MAX_MESSAGE_LENGTH 10

void lab1AppSetup() {
    // Initialize Serial communication and redirect stdio (includes waiting for port ready)
    srvSerialSetup(BAUD_RATE);
    
    // Initialize LED hardware
    ddLedInit(LED_PIN);
    
    // Print welcome message using STDIO (printf)
    printf("====================================\r\n");
    printf("Lab 1: Serial LED Control\r\n");
    printf("Using STDIO for text communication\r\n");
    printf("====================================\r\n");
    printf("\r\n");
    printf("Available commands:\r\n");
    printf("  led on  - Turn LED on\r\n");
    printf("  led off - Turn LED off\r\n");
    printf("  help    - Show this help message\r\n");
    printf("====================================\r\n");
    printf("\r\n");
    printf("System ready. Awaiting commands...\r\n");
    printf("\r\n");
    printf("> ");
}

static void lab1HandleLine(const char* line) {
    char command[MAX_MESSAGE_LENGTH] = {0};
    char modifier[MAX_MESSAGE_LENGTH] = {0};

    // Parse up to two tokens from the input line
    int count = sscanf(line, "%9s %9s", command, modifier);
    if (count <= 0) {
        return;  // Empty line, ignore
    }

    // Convert both tokens to lowercase for case-insensitive matching
    for (int i = 0; command[i]; i++) {
        command[i] = (char)tolower((unsigned char)command[i]);
    }
    for (int i = 0; modifier[i]; i++) {
        modifier[i] = (char)tolower((unsigned char)modifier[i]);
    }

    // Handle 'help' command
    if (strcmp(command, "help") == 0) {
        printf("Available commands:\r\n");
        printf("  led on  - Turn LED on\r\n");
        printf("  led off - Turn LED off\r\n");
        printf("  help    - Show this help message\r\n\r\n");
        return;
    }

    // Handle 'led on' and 'led off' commands
    if (strcmp(command, "led") == 0 && count >= 2) {
        if (strcmp(modifier, "on") == 0) {
            ddLedOn(LED_PIN);
            printf("LED turned ON\r\n\r\n");
        } else if (strcmp(modifier, "off") == 0) {
            ddLedOff(LED_PIN);
            printf("LED turned OFF\r\n\r\n");
        } else {
            printf("ERROR: Unknown modifier '%s'\r\n\r\n", modifier);
        }
        return;
    }

    // Unknown command - provide error feedback
    printf("ERROR: Unknown command '%s'\r\n", command);
    printf("Type 'help' for available commands\r\n\r\n");
}

void lab1AppLoop() {
    static char line[MAX_MESSAGE_LENGTH];  // Line buffer persists across calls
    static size_t index = 0;               // Current position in buffer

    // Process all available characters
    while (Serial.available() > 0) {
        int c = getchar();  // Read one character with STDIO
        // Handle Enter key (CR or LF)
        if (c == '\r' || c == '\n') {
            if (index > 0) {
                line[index] = '\0';     // Null-terminate the line
                putchar('\r');
                putchar('\n');
                lab1HandleLine(line);    // Process the command
                index = 0;               // Reset buffer for next line
            }
            printf("> ");               // Print prompt for next command
            continue;
        }

        // Handle Backspace (\b or DEL)
        if (c == '\b' || c == 127) {
            if (index > 0) {
                index--;                 // Remove character from buffer
                printf("\b \b");         // Erase character on terminal
            }
            continue;
        }

        // Regular character - add to buffer and echo
        if (index < sizeof(line) - 1) {
            line[index++] = (char)c; // Store character
            putchar(c);              // Echo back to terminal
        }
    }
}
