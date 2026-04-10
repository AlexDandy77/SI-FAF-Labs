#ifndef DD_MOTOR_DRIVER_H
#define DD_MOTOR_DRIVER_H

void ddMotorDriverInit(int forwardPin, int reversePin);
void ddMotorDriverSetPercent(int speedPercent);
void ddMotorDriverStop();

#endif
