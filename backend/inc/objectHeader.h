#ifndef objectHeader_h
#define objectHeader_h

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "dlc.h"

#define OBJECT_HEADER_1_START 15
#define QUALIFER_START 17
#define RANGE_START 18

typedef struct dnp3objh_sd {
    uint8_t group;
    uint8_t variation;

    uint8_t qualPrefix:4;
    uint8_t qualRangeCode:4;

    uint64_t range;     // can be upto 8 octetes so we might need this
} dnp3objh_st;

dnp3objh_st mkObjectHeader(uint8_t[]);
void printObjectHeader(dnp3objh_st);

#endif