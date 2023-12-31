#include <ESP8266WiFi.h>
#include <config.h>
#include <motor.h>

uint8_t currentStrInput[NUM_PINS];

uint8_t attenuationSpeed = 20;
uint32_t attenuationDelay = 10000;

uint8_t CalculateAttenuation(uint8_t currentStrength, uint lastUpdate)
{
  static unsigned long LastAttenuation = 0;
  unsigned long sinceLastUpdate = millis() - lastUpdate;
  
  // update according to the refresh rate.
  if ((millis() - LastAttenuation) < (1000 / FALLOFF_REFRESH_RATE)) 
  {
    return currentStrength;
  }

  if (currentStrength == 0){return 0;}

  if (sinceLastUpdate < attenuationDelay)
  {
    Serial.print("Not reached attenuation delay yet \n");
    return currentStrength;
  }


  // Calculate how much to decrease the strength this loop
  // the amount of time that has elapsed since we last attenuated
  float timeElapsed = (float)(millis() - LastAttenuation) * 0.001;
  // speed * delta_time
  uint8_t attenuationValue = floor((float)attenuationSpeed * timeElapsed);
  
  Serial.print("Attenuation value: ");
  Serial.println(attenuationValue);

  // Integer overflow! Yay!
  if (attenuationValue > currentStrength)
  {
    Serial.print("Reached negative strength \n");
    LastAttenuation = millis();
    return 0;
  }

  Serial.print("Ramping down to: ");
  Serial.println(currentStrength - attenuationValue);
  LastAttenuation = millis();
  return currentStrength - attenuationValue;
}

void WriteToMotor(size_t motorID, uint8_t Str)
{
  static unsigned long lastTimeUpdated[NUM_PINS];
  static uint8_t currentMotorOutput[NUM_PINS];

  if (currentStrInput[motorID] != Str)
  {
    // if the input we recieve is new, update
    // analogWrite(PWM_PINS[motorID], Str);
    pwm.setPWM(PWM_PIN_MAPPING[motorID], 0, (Str * 16));
    currentStrInput[motorID] = Str;
    lastTimeUpdated[motorID] = millis();
    currentMotorOutput[motorID] = Str;
  }
  else
  {
    // if the input we recieve is still the same, apply attenuation
    currentMotorOutput[motorID] = CalculateAttenuation(currentMotorOutput[motorID],
                                                       lastTimeUpdated[motorID]);

    // analogWrite(PWM_PINS[motorID], currentMotorOutput[motorID]);
    pwm.setPWM(PWM_PIN_MAPPING[motorID], 0, (currentMotorOutput[motorID] * 16));
  }
}

void UpdateMotorStrength(uint8_t *StrArray, size_t length)
{
  Serial.print("Received data: ");
  for (size_t i = 0; i < length; i++)
  {
    Serial.print(StrArray[i]);
    Serial.print(" ");

    // Set PWM pins based of of recieved strength values
    if (i < NUM_PINS)
    {
      WriteToMotor(i, StrArray[i]);
    }
  }
  Serial.print(" | ");
}

void UpdateAttenuationFunc(struct Attenuation_Control control)
{
  attenuationDelay = control.AttenuationTime;
  attenuationSpeed = control.AttenuationStrength;
}

void CheckStrAttenuation()
{
  for (size_t i = 0; i < NUM_PINS; i++)
  {
    WriteToMotor(i, currentStrInput[i]);
  }
}