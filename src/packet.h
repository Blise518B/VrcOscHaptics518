#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <config.h>

enum PACKET_TYPE
{
  Motor_Strength = 0x00,
  Attenuation_Control = 0x01
};

struct Attenuation_Control
{
  uint32_t AttenuationTime;
  uint8_t AttenuationStrength;
} __attribute__((packed));

typedef struct Payload
{
  uint8_t tag;
  size_t data_len;
  uint8_t *data;
} payload_t;

#endif