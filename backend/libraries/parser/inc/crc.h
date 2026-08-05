#include <stdint.h>

#include "rawPacketHelper.h"

#ifndef crc_h
#define crc_h

#define CRC_HEADER 8
#define CRC_REG_CHUNK 16


uint16_t calculateCRC(uint8_t*, int);
void removeCRC(uint8_t [], uint8_t []);

#endif