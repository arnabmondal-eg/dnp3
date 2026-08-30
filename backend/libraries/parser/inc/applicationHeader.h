#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "dlc.h" 
#include "binaryHelper.h"
#include "dlc.h"

#ifndef applicationHeader_h
#define applicationHeader_h

// position in an input packet (assuming each index is 1 byte long)
#define APPLHDR_START 11

typedef struct application_header_sd {
    // application control bits (ac)
    uint8_t acFragmentSequence:4;
    uint8_t acUnsolicited:1;
    uint8_t acApplConfirm:1;
    uint8_t acFinal:1;
    uint8_t acFirst:1;

    uint8_t applicationFunctionCode;

} __attribute__((packed)) application_header_st;

typedef struct inn_sd {
    // internal indications
    uint8_t inn1;
    uint8_t inn2;
} __attribute__((packed)) inn_st;

application_header_st application_header(uint8_t [], inn_st*, int*, dlc_st*, int*);
void print_application_header(application_header_st, inn_st, int);
void getApplicationHeaderBit(application_header_st);

#endif