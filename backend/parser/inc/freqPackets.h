#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#include "packet.h"

#ifndef freqPackets_h
#define freqPackets_h

typedef struct dnp3Lib_resetLinkP_sd {
    uint16_t start; // 0x05 0x64
    uint8_t length;
    uint8_t dlc;

    uint16_t destination;
    uint16_t source;

    uint16_t crc;
} dnp3Lib_resetLinkP_st;

typedef struct dnp3Lib_ackP_sd {
    uint16_t start; // 0x05 0x64
    uint8_t length;
    uint8_t dlc;

    uint16_t destination;
    uint16_t source;

    uint16_t crc;
} dnp3Lib_ackP_st;

typedef struct dnp3Lib_nackP_sd {
    uint16_t start; // 0x05 0x64
    uint8_t length;
    uint8_t dlc;

    uint16_t destination;
    uint16_t source;

    uint16_t crc;
} dnp3Lib_nackP_st;

dnp3Lib_resetLinkP_st dnp3Lib_mkResetLink(int, int);
dnp3Lib_ackP_st dnp3Lib_mkAck(int, int, int);
dnp3Lib_ackP_st dnp3Lib_mkNack(int, int, int);

#endif