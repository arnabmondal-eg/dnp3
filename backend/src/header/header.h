#ifndef header_h
#define header_h

#include <stdbool.h>    // C23 introduces booleans nativley, but clang doesnt auto compile to 23, so lets just do this :/
#include <stdint.h>

// checks start bytes and crc byte to determine packet validity
bool checkHeaderValidity(char headerRawHex[]);

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

#endif