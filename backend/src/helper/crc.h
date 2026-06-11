#ifndef crc_h
#define crc_h

#include <stdint.h>

#define CRC_HEADER 8
#define CRC_REG_CHUNK 16


int calculateCRC(uint8_t rawHex[], int length);

#endif