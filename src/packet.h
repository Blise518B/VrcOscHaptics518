#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

enum PACKET_TYPE {
    Motor_Strength = 0x00,
    Attenuation_Control = 0x01
};

struct Attenuation_Control {
    uint32_t AttenuationTime;
    uint8_t AttenuationStrength;   
}__attribute__((packed));

#endif