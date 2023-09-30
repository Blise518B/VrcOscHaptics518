#ifndef NETWORK_H
#define NETWORK_H

#include <stddef.h>

// setup wifi and begin listening on the correct port
void InitNetwork(const char *hostname, const char *SSID, const char *password);

// check for network events and time packets
// returns true if a packet was recieved in this loop
bool LoopSocket();

#endif