#ifndef dlc_h
#define dlc_h

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "binaryHelper.h"

#define DEBUG 1

#define DLC_START 3

typedef struct dlc_sd {
    uint8_t fncCodeBits:4;    // 4 bits
    uint8_t fcv_dfcBit:1; // either dfc or fcv depending on packet prm
    uint8_t fcbBit:1;     // alternates to track message sequence
    uint8_t prmBit:1;     // wether inital or response
    uint8_t dirBit:1;     // wether from primary or secondary

    int response;
} dlc_st;

dlc_st mkDLC(uint8_t[]);

void printDLCData(dlc_st);

int getDLCBit(int, dlc_st);


#endif