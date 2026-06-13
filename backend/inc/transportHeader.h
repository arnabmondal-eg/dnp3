#ifndef transportHeader_h
#define transportHeader_h

#include <stdio.h>
#include <stdint.h>

#include "binaryHelper.h"

#define DEBUG 0

//**Positon of TH in all Valid dnp3 packets (i hope) */
#define TRANSPORT_HEADER_POSTION 10

typedef struct dnp3th_sd {
    /**
     * @brief Wether the current packet is first in the sequence
     */
    int first;
    /**
     * @brief Wether the current packet is last in the sequence
     */
    int final;
    /**
     * @brief Packet Sequence Number
     */
    uint8_t segmentSequence;

} dnp3th_st;

dnp3th_st mkTransportHeader(uint8_t [], int);
void printTransportHeader(dnp3th_st);
int getTransportHeaderBit(dnp3th_st);

#endif