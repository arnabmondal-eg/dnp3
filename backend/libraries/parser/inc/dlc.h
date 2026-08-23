#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "binaryHelper.h"

#ifndef dlc_h
#define dlc_h

#define DEBUG 1

#define DLC_START 3

typedef struct dlc_sd {
    uint8_t function_code:4;    // Contains function code , 0 if from outstation (4 bits)
    uint8_t fcv_dfc:1;          // Contians frame count bit validity, dataflow control if from outstation (1 bit)
    uint8_t fcb:1;              // Alternates to track message order, or contains 0 if from out station (1 bit)
    uint8_t prm:1;              // Indicates wether primary or responding packet (1 bit)
    uint8_t dir:1;              // Continans wether comming from Master or Remote (1 bit)
} dlc_st;

dlc_st mkDLC(uint8_t[], int*);

void printDLCData(dlc_st);

int getDLCBit(int, dlc_st);


#endif