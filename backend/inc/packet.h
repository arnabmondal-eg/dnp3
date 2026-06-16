#ifndef packet_h
#define packet_h

#include <stdio.h>
#include <stdint.h>

#include "mainHeader.h"
#include "dlc.h"
#include "transportHeader.h"
#include "applicationHeader.h"

typedef struct dnp3p_sd {
    //TODO: Add other portions of packet
    uint8_t *hexInput;

    dnp3h_st header_s;    // header portion of packet
    dnp3hDLC_st dlc_s;
    dnp3th_st transportHeader_s;
    dnp3aph_st applicationHeader_s;
} dnp3p_st;

dnp3p_st mkPacket(uint8_t [], int packetSize);

void printPacket(dnp3p_st);

#endif