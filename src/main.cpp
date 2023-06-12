#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

const char* ssid = "VR-Gehirnwaescheeinheit24";
const char* password = "LidBd21J";
const int webSocketPort = 8080;

WebSocketsServer webSocket = WebSocketsServer(webSocketPort);
unsigned long prevPacketTime = 0;
bool packetReceived = false;

const int pwmPins[] = {D1, D2, D5, D6, D7, D8};
const int numPins = sizeof(pwmPins) / sizeof(pwmPins[0]);

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
      Serial.print((char)payload[i]);
      Serial.print(" ");

      // Enable or disable PWM pins based on received boolean values
      if (payload[i] == '1' && i < numPins) {
        analogWrite(pwmPins[i], 128);  // 50% duty cycle (128/255)
      } else if (payload[i] == '0' && i < numPins) {
        analogWrite(pwmPins[i], 0);  // 0% duty cycle
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

  WiFi.hostname("ESP_L_Arm");
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
    analogWrite(pwmPins[i], 0);
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