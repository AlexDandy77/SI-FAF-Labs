#include "Arduino.h"
#include "srvSerialStdio.h"

// stream for stdio
static FILE serialOutput = { 0 };
static FILE serialInput = { 0 };

void srvSerialSetup(int baudRate) {
    // Initialize Serial communication
    Serial.begin(baudRate);
    
    // Wait for Serial port to be ready (important for native USB boards)
    while (!Serial) {
        ; // Wait for serial port to connect
    }
    
    fdev_setup_stream(&serialOutput, srvSerialWriteByte, NULL, _FDEV_SETUP_WRITE);
    fdev_setup_stream(&serialInput, NULL, srvSerialReadByte, _FDEV_SETUP_READ);
    stdout = &serialOutput;
    stdin = &serialInput;
}

int srvSerialReadByte(FILE* stream) {
    // Wait until a character is available
    while (!Serial.available());
    // Read and return the character from the serial buffer
    return Serial.read();
}

int srvSerialWriteByte(char c, FILE* stream) {
    // Write the character to the serial port
    Serial.write(c);
    return 0;
}

// ========================================
// Bonus Helper Functions Implementation
// ========================================

int srvSerialAvailable() {
    // Returns the number of bytes available to read from serial buffer
    return Serial.available();
}

void srvSerialFlush() {
    // Wait for outgoing serial data to complete transmission
    Serial.flush();
}

void srvSerialPrintString(const char* str) {
    // Convenience function to print a string using printf
    printf("%s", str);
}

int srvSerialReadLine(char* buffer, int maxLength) {
    int index = 0;
    
    // Read characters until newline or buffer full
    while (index < maxLength - 1) {
        // Wait for character to be available
        while (!Serial.available()) {
            ; // Wait
        }
        
        int c = Serial.read();
        
        // Check for line endings
        if (c == '\r' || c == '\n') {
            // Skip any following line ending characters
            delay(10); // Small delay to catch \r\n pairs
            while (Serial.available()) {
                int next = Serial.peek();
                if (next == '\r' || next == '\n') {
                    Serial.read(); // Consume it
                } else {
                    break;
                }
            }
            break;
        }
        
        // Store character in buffer
        buffer[index++] = (char)c;
    }
    
    // Null-terminate the string
    buffer[index] = '\0';
    return index;
}