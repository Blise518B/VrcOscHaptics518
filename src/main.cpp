#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <config.h>
#include <motor.h>
#include <network.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

void setup()
{
  Serial.begin(115200);
  delay(200);
  Serial.println(" ");
  Serial.print("Connecting to WiFi");

  InitNetwork(WIFI_HOSTNAME, SSID, PASSWORD);

  // Initialize PWM pins
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(1600);
  Wire.setClock(400000);
  for (size_t i = 0; i < NUM_MOTORS; i++)
  {
    // pinMode(PWM_PINS[i], OUTPUT);
    WriteToMotor(i, 0);
  }
}

void loop()
{
  bool updated_packet = LoopSocket();

  // if we didn't recieve a packet this loop,
  // check if haptic strength needs to be lowered
  if (!updated_packet)
  {
    CheckStrAttenuation();
  }

#ifdef ESP8266
  //yield(); // take a breather, required for ESP8266
#endif
}