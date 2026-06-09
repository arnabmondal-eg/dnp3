#ifndef header_h
#define header_h

#include <stdbool.h>    // C23 introduces booleans nativley, but clang doesnt auto compile to 23, so lets just do this :/

// checks start bytes and crc byte to determine packet validity
bool checkPacketValidity(char headerRawHex[11]);

// total 10 bytes / 3 compiler blocks (2 unsed bytes)
typedef struct dnp3h_sd {
    int s1:1;   // start byte 1
    int s2:1;   // start byte 2

    int len:1;   // length byte

    int dlc:1;  // data link control byte

    int des:2;  // destination byte

    int src:2;  // source byte

    int crc:2;  // cyclic redundancy check byte
} dnp3h_st;

#endif