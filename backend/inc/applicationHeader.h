#ifndef applicationHeader_h
#define applicationHeader_h

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "mainHeader.h"
#include "dlc.h" 
#include "binaryHelper.h"

// position in an input packet (assuming each index is 1 byte long)
#define APPLHDR_START 11

typedef struct dnp3aph_sd {
    // application control bits (ac)
    uint8_t acFragmentSequence:4;
    uint8_t acUnsolicited:1;
    uint8_t acApplConfirm:1;
    uint8_t acFinal:1;
    uint8_t acFirst:1;

    uint8_t applicationFunctionCode;

    // internal indications
    uint8_t inn1;  // gota read these bit by bit
    uint8_t inn2;   // each bit indicates something (im gonna die)
    int innActive;
    
} dnp3aph_st;

dnp3aph_st mkApplicationHeader(uint8_t []);
void printApplicationHeader(dnp3aph_st);
void getApplicationHeaderBit(dnp3aph_st);

#endif