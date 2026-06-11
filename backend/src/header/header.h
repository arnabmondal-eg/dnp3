#ifndef header_h
#define header_h

#include <stdint.h>

// dlc defs: primary to secondary function codes
// src: dnp3 spec v4 data link layer rev. 2007 02 03
#define DLC_RESET_LINK_STATES 0     // reset of remote link
#define DLC_TEST_LINK_STATES 2      // test function for link
#define DLC_CONFIRMED_USER_DATA 3   // deliver application data, confirmation requested
#define DLC_UNCONFIRMED_USER_DATA 4 // deliver application data, confirmation not requested
#define DLC_REQUEST_LINK_STATUS 9   // request status of link

// total 10 bytes / 3 compiler blocks (2 unsed bytes)
typedef struct dnp3h_sd {
    uint8_t s1;   // start byte 1
    uint8_t s2;   // start byte

    uint8_t len;   // length byte
    uint8_t dlc;  // data link control byte

    uint16_t des;  // destination byte

    uint16_t src;  // source byte

    uint16_t crc;  // cyclic redundancy check byte
} dnp3h_st;

extern dnp3h_st mkHeader(char []);
// checks start bytes and crc byte to determine packet validity
extern int checkHeaderValidity(dnp3h_st);

extern void extractDLC(dnp3h_st);

extern void printHeader(dnp3h_st);

#endif