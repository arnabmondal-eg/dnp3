#include <stdio.h>
#include <string.h>

#include "header.h"
#include "helper/crcHelper/crc.h"
#include "helper/splitByteHelper/splitTwoByte.h"

dnp3h_st mkHeader(char headerRawHex[]) {
    dnp3h_st header;

    memcpy(&header, headerRawHex, sizeof(header));

    return header;
}

/** ~Asthetic~ Print. Just for testing. 
 * @param header The header to be decomposed and printed
 * 
 * 
 * Prints all contents (start, length, dlc, ect...) hex, and for specific values (length, destination, source) prints decimal
*/
void printHeader(dnp3h_st header) {
    printf("\n---- Packet Header ----\n");
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
    int valid = 0;     // assume invalid

    // check for start bytes
    if (header.s1 == 0x05 && header.s2 == 0x64) {
        valid = 1;
    }
    else {
        valid = 0;
    }

    // check crc bytes

    // extract hex data without crc
    uint8_t checkHex[8] = {header.s1, header.s2, 
        header.len, header.dlc, 
        splitTwoByte(header.des, 0), splitTwoByte(header.des, 1),       // split these 2 byte hexes
        splitTwoByte(header.src, 0), splitTwoByte(header.src, 1)        // into single bytes
    };

    // if already false, dont set back to true
    valid = header.crc == calculateCRC(checkHex, CRC_HEADER) && valid == 1 ? 1 : 0;

    return valid;
}