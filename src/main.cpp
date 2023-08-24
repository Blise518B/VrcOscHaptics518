#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <config.h>
#include <motor.h>
#include <network.h>

void setup()
{
  Serial.begin(115200);
  delay(200);
  Serial.println(" ");
  Serial.print("Connecting to WiFi");

  InitNetwork(WIFI_HOSTNAME, SSID, PASSWORD);

  // Initialize PWM pins
  for (size_t i = 0; i < NUM_PINS; i++)
  {
    pinMode(PWM_PINS[i], OUTPUT);
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
}