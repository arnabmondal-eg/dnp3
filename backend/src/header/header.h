#ifndef header_h
#define header_h

#include <stdint.h>

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

// dlc mask
#define DLC_FUNCTION_CODE_MASK 0x0F

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

extern dnp3h_st mkHeader(uint8_t []);
// checks start bytes and crc byte to determine packet validity
extern int checkHeaderValidity(dnp3h_st);

extern void extractDLC(dnp3h_st);

extern void printHeader(dnp3h_st);

#endif