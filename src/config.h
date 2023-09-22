#ifndef CONFIG_H
#define CONFIG_H

// Network Information
const char SSID[] = "VR-Gehirnwaescheeinheit24";
const char PASSWORD[] = "LidBd21J";
// ESP-L-Arm / ESP-F-Body / ESP-R-Body / ESP-R-Arm / ESP-Head / ESP-L-Leg / ESP-R-Leg / ESP-Extra
const char WIFI_HOSTNAME[] = "ESP-L-Arm";
const int WEB_SOCKET_PORT = 8080;

// Motor Configuration
const int PWM_PINS[] = {D1, D2, D5, D6, D7, D8};
const size_t NUM_PINS = sizeof(PWM_PINS) / sizeof(PWM_PINS[0]);
const unsigned long ATTENUATION_TIME = 10000;

const int PWM_PIN_MAPPING[] = {0, 2, 4, 6, 8, 10}; // New way of assigning pins
const size_t NUM_MOTORS = sizeof(PWM_PIN_MAPPING) / sizeof(PWM_PIN_MAPPING[0]);
#endif