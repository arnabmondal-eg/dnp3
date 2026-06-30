#ifndef header_h
#define header_h

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "crc.h"
#include "splitTwoByte.h"
#include "binaryHelper.h"

#define DEBUG 1


// total 10 bytes / 3 compiler blocks (2 unsed bytes)
typedef struct header_sd {
    uint8_t s1;   // start byte 1
    uint8_t s2;   // start byte

    uint8_t len;   // length byte
    uint8_t dlc;  // data link control byte

    uint16_t des;  // destination byte

    uint16_t src;  // source byte

    uint16_t crc;  // cyclic redundancy check byte
} header_st;

header_st mkHeader(uint8_t [], int, int*);
// checks start bytes and crc byte to determine packet validity
int checkHeaderValidity(header_st);

void printHeader(header_st);

#endif