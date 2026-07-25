#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#include "header.h"
#include "dlc.h"
#include "transportHeader.h"
#include "applicationHeader.h"
#include "objectHeader.h"
#include "data.h"

#ifndef packet_h
#define packet_h

/* Defines */

/* Groups */
#define CONTINUOUS 30
#define BINARY 1

/* Varriations */   // techinacly these are useless but idc
#define VAR_1 1
#define VAR_2 2

typedef struct dnp3p_sd {
    //TODO: Add other portions of packet
    uint8_t *hexInput;

    header_st header_s;
    dlc_st dlc_s;
    transportHeader_st transportHeader_s;
    applicationHeader_st applicationHeader_s;
    objectHeader_st objectHeader_s;

    int packetValidity;
    int caretPosition;
} dnp3p_st;

dnp3p_st mkPacketAuto(uint8_t[]);

/**
 * @brief Creates `dnp3p_st` based on input params
 * 
 * @return dnp3p_st 
 */
dnp3p_st dnp3lib_mkpacket_manual(int, int, int, int, int, int);
void printPacket(dnp3p_st);

#endif