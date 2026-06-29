#include "header.h"

/**
 * @brief Creates a dnp3 Header data type
 * 
 * @param hexInput Hex Sequence to Transform
 * @return dnp3h_st New var
 */
dnp3h_st mkHeader(uint8_t hexInput[], int inputSize, int *packetValidity) {
    dnp3h_st header_s = {0};

    if (inputSize < 10) *packetValidity = 0;
    else *packetValidity = 1;
    memcpy(&header_s, hexInput, sizeof(header_s));

    return header_s;
}

/**
 * Checks Header Portion of Packet for Validity
 * @param hexInput Chacter Array: The Header to Check
 * 
 * @returns Boolean: Based on 2 Checks; Start Byte and CRC Byte Check
 */
int checkHeaderValidity(dnp3h_st header_s) {
    int valid = 0;     // assume invalid

    // check for start bytes
    if (header_s.s1 == 0x05 && header_s.s2 == 0x64) {
        valid = 1;
    }
    else {
        valid = 0;
    }

    // check crc bytes

    // extract hex data without crc
    uint8_t checkHex[8] = {header_s.s1, header_s.s2, 
        header_s.len, header_s.dlc, 
        splitTwoByte(header_s.des, 0), splitTwoByte(header_s.des, 1),       // split these 2 byte ints
        splitTwoByte(header_s.src, 0), splitTwoByte(header_s.src, 1)        // into single bytes
    };

    // if already false, dont set back to true
    valid = header_s.crc == calculateCRC(checkHex, CRC_HEADER) && valid == 1 ? 1 : 0;

    return valid;
}

void printHeader(dnp3h_st header_s) {
    printf("---- Packet Header ----\n");
    printf("Start: 0x%02X 0x%02X\n", header_s.s1, header_s.s2);
    printf("Length: 0x%02X [%d]\n", header_s.len, header_s.len);
    printf("Data Link Control: 0x%02X\n", header_s.dlc);
    printf("Destination: 0x%04X [%d]\n", header_s.des, header_s.des);
    printf("Source: 0x%04X [%d]\n", header_s.src, header_s.src);
    printf("CRC: 0x%04X\n", header_s.crc);
}