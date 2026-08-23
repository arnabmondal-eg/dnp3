#include "header.h"
#include "packet.h"

/**
 * @brief Creates a dnp3 Header data type
 * 
 * @param hexInput Hex Sequence to Transform
 * @return header_st New var
 */
header_st header(uint8_t hexInput[], int *caretPosition) {
    header_st header_s = {0};

    memcpy(&header_s, hexInput, sizeof(header_s));
    *caretPosition += sizeof(header_s);

    return header_s;
}

void mkHeader1(uint8_t hexInput[], dnp3p_st *packet_sp) {
   // header_st header_s = {0};

    memcpy(&packet_sp->header_s, hexInput, sizeof(header_st));
    packet_sp->caretPosition += sizeof(header_st);

    return;
}

/**
 * Checks Header Portion of Packet for Validity
 * @param hexInput Chacter Array: The Header to Check
 * 
 * @returns Boolean: Based on 2 Checks; Start Byte and CRC Byte Check
 */
int checkHeaderValidity(header_st header_s) {
    int valid = 0;     // assume invalid

    // check for start bytes
    if (header_s.s1 == 0x05 && header_s.s2 == 0x64) {
        valid = 1;
    }
    else {
        valid = 0;
    }

    valid = header_s.crc == calculateCRC(&header_s, CRC_HEADER) && valid == 1 ? 1 : 0;

    return valid;
}

void printHeader(header_st header_s) {
    printf("---- Packet Header ----\n");
    printf("Start: 0x%02X 0x%02X\n", header_s.s1, header_s.s2);
    printf("Length: 0x%02X [%d]\n", header_s.len, header_s.len);
    printf("Data Link Control: 0x%02X\n", *(uint8_t *)&header_s.dlc_s);
    printf("Destination: 0x%04X [%d]\n", header_s.des, header_s.des);
    printf("Source: 0x%04X [%d]\n", header_s.src, header_s.src);
    printf("CRC: 0x%04X\n", header_s.crc);
}