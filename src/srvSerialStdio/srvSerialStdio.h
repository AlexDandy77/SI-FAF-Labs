#ifndef SRV_SERIAL_STDIO_H
#define SRV_SERIAL_STDIO_H

#include <stdio.h>
#include <Arduino.h>

// Initialize serial communication, redirect stdio, and wait for port ready
// (important for native USB boards)
void srvSerialSetup(int baudRate);

// Low-level character I/O functions
int srvSerialReadByte(FILE* stream);
int srvSerialWriteByte(char c, FILE* stream);

// ========================================
// Bonus Helper Functions
// ========================================

// Check if data is available to read from serial
int srvSerialAvailable();

// Flush the serial output buffer (wait for transmission to complete)
void srvSerialFlush();

// Print a null-terminated string to serial (convenience wrapper)
void srvSerialPrintString(const char* str);

// Read a line from serial into buffer, returns number of characters read
// Waits for newline (\n or \r), null-terminates the string
// maxLength includes space for null terminator
int srvSerialReadLine(char* buffer, int maxLength);

#endif