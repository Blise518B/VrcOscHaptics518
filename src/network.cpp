#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <motor.h>
#include <packet.h>

WebSocketsServer webSocket = WebSocketsServer(WEB_SOCKET_PORT);
unsigned long prevPacketTime = 0;
bool packetReceived = false;

void webSocketEvent(uint8_t num, WStype_t type,
                    uint8_t *payload, size_t length)
{
  if (type == WStype_BIN)
  {
    unsigned long currentPacketTime = millis();
    packetReceived = true;

    if (prevPacketTime != 0)
    {
      unsigned long timeDiff = currentPacketTime - prevPacketTime;
      Serial.print("Time between packets (ms): ");
      Serial.print(timeDiff);
      Serial.print(" | ");
    }

    prevPacketTime = currentPacketTime;

    switch (*payload)
    {
    case Motor_Strength:
      UpdateMotorStrength(payload + 1, length - 1);
      break;
    case Attenuation_Control:
      if (length - 1 < sizeof(struct Attenuation_Control))
        break;

      struct Attenuation_Control control = *(struct Attenuation_Control *)(payload + 1);
      UpdateAttenuationFunc(control);
      break;
    }
  }
}

void InitNetwork(const char *hostname, const char *SSID,
                 const char *password)
{
  WiFi.hostname(hostname);
  WiFi.begin(SSID, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print(".");
  }

  Serial.println(" ");

  // Print ESP8266 IP address
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void LoopSocket()
{
  unsigned long loopStartTime = millis();

  webSocket.loop();

  unsigned long loopEndTime = millis();
  unsigned long loopTime = loopEndTime - loopStartTime;

  if (packetReceived)
  {
    Serial.print("Total program loop time (ms): ");
    Serial.println(loopTime);
    packetReceived = false;
  }
  else
  {
    CheckStrAttenuation();
  }
}