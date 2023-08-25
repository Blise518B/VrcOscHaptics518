#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <motor.h>
#include <packet.h>

WebSocketsServer webSocket = WebSocketsServer(WEB_SOCKET_PORT);
unsigned long prevPacketTime = 0;
bool packetReceived = false;

// forward declaration
void webSocketEvent(uint8_t num, WStype_t type,
                    uint8_t *payload, size_t length);

void InitNetwork(const char *hostname, const char *SSID,
                 const char *password)
{
  // Start WiFi and check if we are properly connected
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

  // start the websocket and check for events with webSocketEvent
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

bool LoopSocket()
{
  // 
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

  return packetReceived;
}

void webSocketEvent(uint8_t num, WStype_t type,
                    uint8_t *payload, size_t length)
{
  if (type == WStype_BIN)
  {
    // split the packet's tag and it's data
    struct Payload packet;
    packet.tag = *payload;
    packet.data_len = length - 1;
    packet.data = payload + 1;

    // time the packet and print to serial
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

    // handle different the different payload tags
    switch (packet.tag)
    {
    case Motor_Strength:
      UpdateMotorStrength(packet.data, packet.data_len);
      break;
    case Attenuation_Control:
      // make sure we don't copy a control packet of insufficient length
      if (packet.data_len < sizeof(struct Attenuation_Control))
      {
        Serial.print("Malformed control packet recieved.");
        break;
      }

      struct Attenuation_Control *control = (struct Attenuation_Control *)(packet.data);
      UpdateAttenuationFunc(*control);
      break;
    }
  }
}