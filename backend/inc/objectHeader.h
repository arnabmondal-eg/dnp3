#ifndef objectHeader_h
#define objectHeader_h

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "dlc.h"

#define OBJECT_HEADER_1_START 15
#define QUALIFER_START 17
#define RANGE_START 18

typedef struct objectHeader_sd {
    uint8_t group;
    uint8_t variation;

    uint8_t qualPrefix:4;
    uint8_t qualRangeCode:4;

    // uint64_t range;     // can be upto 8 octetes so we might need this

    uint32_t rangeStart;
    uint32_t rangeStop;

    int numberOfPoints; 
} objectHeader_st;

objectHeader_st mkObjectHeader(uint8_t[]);
void printObjectHeader(objectHeader_st);

#endif