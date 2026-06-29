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
typedef struct dnp3h_sd {
    uint8_t s1;   // start byte 1
    uint8_t s2;   // start byte

    uint8_t len;   // length byte
    uint8_t dlc;  // data link control byte

    uint16_t des;  // destination byte

    uint16_t src;  // source byte

    uint16_t crc;  // cyclic redundancy check byte
} dnp3h_st;

dnp3h_st mkHeader(uint8_t [], int, int*);
// checks start bytes and crc byte to determine packet validity
int checkHeaderValidity(dnp3h_st);

void printHeader(dnp3h_st);

#endif