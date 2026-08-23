#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "binaryHelper.h"

#ifndef transportHeader_h
#define transportHeader_h

#define DEBUG 1

//**Positon of TH in all Valid dnp3 packets (i hope) */
#define TRANSPORT_HEADER_POSTION 10

typedef struct transport_header_sd {
    uint8_t segmentSequence:6;
    uint8_t first:1;
    uint8_t final:1;
} transport_header_st;

transport_header_st transport_header(uint8_t [], int*);
void print_transport_header(transport_header_st);
int getTransportHeaderBit(transport_header_st);

#endif