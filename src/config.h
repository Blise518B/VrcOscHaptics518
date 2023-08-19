#ifndef CONFIG_H
#define CONFIG_H

// Network Information
const char SSID[] = "VR-Gehirnwaescheeinheit24";
const char PASSWORD[] = "LidBd21J";
const int WEB_SOCKET_PORT = 8080;

// Motor Configuration
const int PWM_PINS[] = {D1, D2, D5, D6, D7, D8};
const size_t NUM_PINS = sizeof(PWM_PINS) / sizeof(PWM_PINS[0]);
const unsigned long ATTENUATION_TIME = 10000;

#endif