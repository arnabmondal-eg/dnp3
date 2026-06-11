#include <stdio.h>
#include <string.h>

#include "header.h"
#include "helper/crc.h"

dnp3h_st mkHeader(char headerRawHex[]) {
    dnp3h_st header;

    memcpy(&header, headerRawHex, sizeof(header));

    return header;
}

void printHeader(dnp3h_st header) {
    printf("---- Packet Header ----\n");
    printf("Start: 0x%02X 0x%02X\n", header.s1, header.s2);
    printf("Length: 0x%02X [%d]\n", header.len, header.len);
    printf("Data Link Control: 0x%02X\n", header.dlc);
    printf("Destination: 0x%04X [%d]\n", header.des, header.des);
    printf("Source: 0x%04X [%d]\n", header.src, header.src);
    printf("CRC: 0x%04X\n", header.crc);
}

/**
 * Checks Header Portion of Packet for Validity
 * @param headerRawHex Chacter Array: The Header to Check
 * 
 * @returns Boolean: Based on 2 Checks; Start Byte and CRC Byte Check
 */
int checkHeaderValidity(dnp3h_st header) {
    int vaild = 0;     // assume invalid

    // check for start bytes
    if (header.s1 == 0x05 && header.s2 == 0x64) {
        vaild = 1;
    }
    else {
        vaild = 0;
    }

    // check crc bytes

    // char checkHex[8];   // extract hex data without crc
    // for (int i = 0; i < 8; i++) {
    //     checkHex[i] = headerRawHex[i];
    // }

    // if (header.crc == calculateCRC(checkHex, CRC_HEADER) && vaild == true) {
    //     vaild = 1;
    // }
    // else {
    //     vaild = 0;
    // }

    return vaild;
}