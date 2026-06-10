#ifndef header_h
#define header_h

#include <stdbool.h>    // C23 introduces booleans nativley, but clang doesnt auto compile to 23, so lets just do this :/

// checks start bytes and crc byte to determine packet validity
bool checkHeaderValidity(char headerRawHex[]);

// total 10 bytes / 3 compiler blocks (2 unsed bytes)
typedef struct dnp3h_sd {
    u_int8_t s1;   // start byte 1
    u_int8_t s2;   // start byte

    u_int8_t len;   // length byte
    u_int8_t dlc;  // data link control byte

    u_int16_t des;  // destination byte

    u_int16_t src;  // source byte

    u_int16_t crc;  // cyclic redundancy check byte
} dnp3h_st;

#endif