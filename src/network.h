#ifndef NETWORK_H
#define NETWORK_H

#include <stddef.h>

void InitNetwork(const char *hostname, const char *SSID, const char *password);

void LoopSocket();

#endif