#ifndef MOTOR_H
#define MOTOR_H

#include <config.h>
#include <stdint.h>
#include <stddef.h>

void WriteToMotor(size_t motorID, uint8_t Str);
void CheckStrAttenuation();

#endif