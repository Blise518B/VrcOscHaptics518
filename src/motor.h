#ifndef MOTOR_H
#define MOTOR_H

#include <config.h>
#include <stdint.h>
#include <stddef.h>
#include <packet.h>

// Takes a motorID assigned in the PWM_PINS array in config.h.
// Takes a Strength value from 0-255.
// Updates PWM signals to motors and 
// calculates attenuation if the Strength is 
// unchanging for a long time.
void WriteToMotor(size_t motorID, uint8_t Str);

// Updates the attenuation values of motors when it is called
void CheckStrAttenuation();

void UpdateMotorStrength(uint8_t *StrArray, size_t length);

void UpdateAttenuationFunc(struct Attenuation_Control control);

#endif