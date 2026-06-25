#ifndef dlc_h
#define dlc_h

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "mainHeader.h"
#include "binaryHelper.h"

#define DEBUG 1

#define DLC_START 3

// dlc defs: primary to secondary function codes; PRM = 1
// src: dnp3 spec v4 data link layer rev. 2007 02 03
#define DLC_RESET_LINK_STATES 0x00     // reset of remote link
#define DLC_TEST_LINK_STATES 0x02      // test function for link
#define DLC_CONFIRMED_USER_DATA 0x03   // deliver application data, confirmation requested
#define DLC_UNCONFIRMED_USER_DATA 0x04 // deliver application data, confirmation not requested
#define DLC_REQUEST_LINK_STATUS 0x09   // request status of link
// secondary to primary; PRM = 0
#define DLC_ACK 0x00       // positive acknoledgement (confirm type)
#define DLC_NACK 0x01      // negative acknoledgement (confirm type)
#define DLC_LINK_STATUS 0x0B   // status of link (respond type)
#define DLC_NOT_SUPPORTED 0x0F

typedef struct dnp3hDLC_sd {
    uint8_t fncCodeBits:4;    // 4 bits
    uint8_t fcv_dfcBit:1; // either dfc or fcv depending on packet prm
    uint8_t fcbBit:1;     // alternates to track message sequence
    uint8_t prmBit:1;     // wether inital or response
    uint8_t dirBit:1;     // wether from primary or secondary

    int response;
} dnp3hDLC_st;

dnp3hDLC_st mkDLC(uint8_t[]);

void printDLCData(dnp3hDLC_st);

int getDLCBit(int, dnp3hDLC_st);


#endif