#ifndef transportHeader_h
#define transportHeader_h

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "binaryHelper.h"

#define DEBUG 1

//**Positon of TH in all Valid dnp3 packets (i hope) */
#define TRANSPORT_HEADER_POSTION 10

typedef struct transportHeader_sd {
    uint8_t segmentSequence:6;
    uint8_t first:1;
    uint8_t final:1;
} transportHeader_st;

transportHeader_st mkTransportHeader(uint8_t []);
void printTransportHeader(transportHeader_st);
int getTransportHeaderBit(transportHeader_st);

#endif