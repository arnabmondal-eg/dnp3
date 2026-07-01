#ifndef data_h
#define data_h

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "objectHeader.h"

/**
 * @brief Binary Input, Packed Format
 * 
 * Group, Variation (0x): 01 01
 */
typedef struct data0101_sd {
    uint8_t data:1;     // packed format is very effecient :)
} data0101_st;

/**
 * @brief Binary Input, With Flags
 * 
 * Group, Varriation (0x): 01 02
 */
typedef struct data0102_sd {
    uint8_t state:1;
    uint8_t reserved:1;     // always 0
    uint8_t chatterFiler:1;
    uint8_t localForced:1;
    uint8_t remoteForced:1;
    uint8_t commLost:1;
    uint8_t restart:1;
    uint8_t online:1;
} data0102_st;

// modifys mem adress (for that speed)
void* mkData(uint8_t[], void*);
void mkData0101(uint8_t, data0101_st*);
void mkData0102(uint8_t, data0102_st*);

#endif