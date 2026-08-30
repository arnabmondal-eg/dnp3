#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <math.h>

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
    
    header_st header_s;
    transport_header_st transport_header_s;
    application_header_st application_header_s;
    inn_st inn_s;
    objectHeader_st object_header_s;
    
    uint8_t *hexInput;
    
    int packetValidity; 
    int caretPosition;
    int innActive;
    int total_points;
} __attribute__((packed)) dnp3p_st;

typedef struct dnp3_data_request_sd {
    /* Header */
    uint16_t start;             // Contains 0x05 0x64
    uint8_t length;             // contains packet length not including start, length, or any crc bytes
    dlc_st dlc_s;               // contains data link information
    uint16_t source;            // source of packet
    uint16_t destination;       // destination of packet
    uint16_t header_crc;        // crc for header

    /* Data Block Information */
    transport_header_st transport_header;        // information about packet transport
    application_header_st application_header_s;  // information about packet
    objectHeader_st object_header_s;            // information about request
} __attribute__((packed)) dnp3_data_request_st;

typedef struct dnp3_data_reply_sd {
    /* Header */
    uint16_t start;             // Contains 0x05 0x64
    uint8_t length;             // contains packet length not including start, length, or any crc bytes
    dlc_st dlc_s;               // contains data link information
    uint16_t source;            // source of packet
    uint16_t destination;       // destination of packet
    uint16_t header_crc;        // crc for header

    /* Data Block Information */
    transport_header_st transport_header;        // information about packet transport
    application_header_st application_header_s;  // information about packet
    inn_st inn_s;                               // application flags
    objectHeader_st object_header_s;            // information about reply
} __attribute__((packed)) dnp3_data_reply_st;

dnp3p_st mkPacketAuto(uint8_t[]);

/**
 * @brief Creates `dnp3p_st` based on input params
 * 
 * @return dnp3p_st 
 */
dnp3p_st dnp3lib_mkpacket_manual(int, int, int, int, int, int);
void printPacket(dnp3p_st);

#endif