#ifndef transportHeader_h
#define transportHeader_h

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "binaryHelper.h"

#define DEBUG 1

//**Positon of TH in all Valid dnp3 packets (i hope) */
#define TRANSPORT_HEADER_POSTION 10

typedef struct dnp3th_sd {
    uint8_t segmentSequence:4;
    uint8_t final:1;
    uint8_t first:1;
} dnp3th_st;

dnp3th_st mkTransportHeader(uint8_t [], int);
void printTransportHeader(dnp3th_st);
int getTransportHeaderBit(dnp3th_st);

#endif