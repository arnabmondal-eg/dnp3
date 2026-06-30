#ifndef packet_h
#define packet_h

#include <stdio.h>
#include <stdint.h>

#include "header.h"
#include "dlc.h"
#include "transportHeader.h"
#include "applicationHeader.h"
#include "objectHeader.h"

typedef struct dnp3p_sd {
    //TODO: Add other portions of packet
    uint8_t *hexInput;

    header_st header_s;
    dlc_st dlc_s;
    transportHeader_st transportHeader_s;
    applicationHeader_st applicationHeader_s;
    objectHeader_st objectHeader_s;

    int packetValidity;
} dnp3p_st;

dnp3p_st mkPacket(uint8_t [], int packetSize);

void printPacket(dnp3p_st);

#endif