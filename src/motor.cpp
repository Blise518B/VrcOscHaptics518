#include <ESP8266WiFi.h>
#include <config.h>
#include <motor.h>

void WriteToMotor(size_t motorID, uint8_t Str) {
  static unsigned long lastTimeUpdated[numPins];
  static uint8_t currentMotorOutput[numPins];

  if (currentStrInput[motorID] != Str) {
    // if the input we recieve is new, update
    analogWrite(pwmPins[motorID], Str);
    currentStrInput[motorID] = Str;
    lastTimeUpdated[motorID] = millis();
    currentMotorOutput[motorID] = Str;
  } else {
    // if the input we recieve is still the same, apply attenuation
    currentMotorOutput[motorID] = (currentMotorOutput[motorID], 
      lastTimeUpdated[motorID]);
    
    analogWrite(pwmPins[motorID], currentMotorOutput[motorID]);
  }
}

uint8_t CalculateAttenuation(uint8_t currentStrength, uint lastUpdate) {
  unsigned long sinceLastUpdate = millis() - lastUpdate;
  uint8_t attenuationValue;

  if (sinceLastUpdate < attenuationTime) {
    return currentStrength;
  }

  attenuationValue = 20 * floor((float)(sinceLastUpdate - attenuationTime) * 0.001);

  if (attenuationValue > currentStrength) {
    return 0;
  }

  return currentStrength - attenuationValue;
}

void CheckStrAttenuation() {
  for(int i = 0; i <= numPins; i++) {
    WriteToMotor(i, currentStrInput[i]);
  }
}