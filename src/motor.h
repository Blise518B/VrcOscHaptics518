#ifndef MOTOR_H
#define MOTOR_H

#include <config.h>
#include <stdint.h>
#include <stddef.h>

uint8_t currentStrInput[numPins];

void WriteToMotor(size_t motorID, uint8_t Str);
void CheckStrAttenuation();

#endif