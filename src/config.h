// Network Information
const char* ssid = "VR-Gehirnwaescheeinheit24";
const char* password = "LidBd21J";
const int webSocketPort = 8080;

// Motor Configuration
const int pwmPins[] = {D1, D2, D5, D6, D7, D8};
const int numPins = sizeof(pwmPins) / sizeof(pwmPins[0]);