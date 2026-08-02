#ifndef client_h
#define client_h

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/select.h>

#include "packet.h"
#include "freqPackets.h"
#include "rawPacketHelper.h"

#define PORT 48188  // https://www.random.org/ (0-65535)

void recivePacket(int, uint8_t []);

void processDLC(uint8_t[], uint8_t[]);

#endif