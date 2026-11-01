#include <stdio.h>
#include <stdint.h>

#ifndef rawPacketHelper_h
#define rawPacketHelper_h

int getPacketSize(const uint8_t[]);
void printRawPacket(uint8_t[]);

#endif