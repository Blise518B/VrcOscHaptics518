#ifndef CONFIG_H
#define CONFIG_H

// Network Information
const char* ssid = "VR-Gehirnwaescheeinheit24";
const char* password = "LidBd21J";
const int webSocketPort = 8080;

// Motor Configuration
const int pwmPins[] = {D1, D2, D5, D6, D7, D8};
const size_t numPins = sizeof(pwmPins) / sizeof(pwmPins[0]);
const unsigned long attenuationTime = 10000;

#endif