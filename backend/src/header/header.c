#include <stdio.h>
#include <string.h>

#include "header.h"
#include "helper/crc.h"

/**
 * Checks Header Portion of Packet for Validity
 * @param headerRawHex Chacter Array: The Header to Check
 * 
 * @returns Boolean: Based on 2 Checks; Start Byte and CRC Byte Check
 */
bool checkHeaderValidity(char headerRawHex[]) {
    bool vaild = false;     // assume invalid
    dnp3h_st header;

    memcpy(&header, headerRawHex, sizeof(header));   // map chars to header struct
    
    // check for start bytes
    if (header.s1 == 0x05 && header.s2 == 0x64) {
        vaild = true;
        // printf("Packet Vaild\n");
    }
    else {
        vaild = false;
        // printf("Packet Invaild\nStart Byte 1 Returned %d rather than 0x05\nStart Byte 2 Returned %d rather than 0x64", header.s1, header.s2);
    }


    // check crc bytes

    char checkHex[8];   // extract hex data without crc
    for (int i = 0; i < 8; i++) {
        checkHex[i] = headerRawHex[i];
    }

    if (header.crc == calculateCRC(checkHex, CRC_HEADER) && vaild == true) {
        vaild = true;
    }
    else {
        vaild = false;
    }

    return vaild;
}