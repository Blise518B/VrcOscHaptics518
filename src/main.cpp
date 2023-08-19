#include <ESP8266WiFi.h> // Neds to change to ESP 32 !!!!!!!!!!!!!!!!!!!!!!!!!
#include <WebSocketsServer.h>
#include <config.h>

WebSocketsServer webSocket = WebSocketsServer(webSocketPort);
unsigned long prevPacketTime = 0;
bool packetReceived = false;

static void WriteToMotor(int motorID, uint8_t Str) {
  uint lastTimeUpdated[numPins];
  uint8_t currentPinOutput[numPins];

  if (currentPinOutput[motorID] != Str) {
    analogWrite(pwmPins[motorID], Str);
    currentPinOutput[motorID] = Str;
    lastTimeUpdated[motorID] = millis();
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    unsigned long currentPacketTime = millis();
    packetReceived = true;

    if (prevPacketTime != 0) {
      unsigned long timeDiff = currentPacketTime - prevPacketTime;
      Serial.print("Time between packets (ms): ");
      Serial.print(timeDiff);
      Serial.print(" | ");
    }

    prevPacketTime = currentPacketTime;

    Serial.print("Received data: ");
    for (size_t i = 0; i < length; i++) {
      Serial.print(payload[i]);
      Serial.print(" ");

      // Set PWM pins based of of recieved strength values
      if (i < numPins) {
        WriteToMotor(i, payload[i]);
      }
    }
    Serial.print(" | ");
  }
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println(" ");
  Serial.print("Connecting to WiFi");

  WiFi.hostname("ESP-L-Arm"); // Neds to change to current body position: ESP-L-Arm / ESP-F-Body / ESP-R-Body / ESP-R-Arm / ESP-Head / ESP-L-Leg / ESP-R-Leg / ESP-Extra
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println(" ");

  // Print ESP8266 IP address
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // Initialize PWM pins
  for (int i = 0; i < numPins; i++) {
    pinMode(pwmPins[i], OUTPUT);
    WriteToMotor(i, 0);
  }
}

void loop() {
  unsigned long loopStartTime = millis();

  webSocket.loop();

  unsigned long loopEndTime = millis();
  unsigned long loopTime = loopEndTime - loopStartTime;

  if (packetReceived) {
    Serial.print("Total program loop time (ms): ");
    Serial.println(loopTime);
    packetReceived = false;
  }
}