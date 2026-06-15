#ifndef packet_h
#define packet_h

#include "header.h"   // look in src/header

dnp3h_st getHeader();

char getFullHex();

char makeNewHex();

typedef struct dnp3p_sd {
    //TODO: Add other portions of packet
    dnp3h_st header;    // header portion of packet
    // dnp3b_st body;
} dnp3p_st;

#endif