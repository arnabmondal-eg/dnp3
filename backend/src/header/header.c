#include <stdio.h>
#include <string.h>

#include "header.h"
#include "helper/crcHelper/crc.h"
#include "helper/splitByteHelper/splitTwoByte.h"
#include "helper/binaryHelper/binaryHelper.h"



/**
 * @brief Creates a dnp3 Header data type
 * 
 * @param headerRawHex Hex Sequence to Transform
 * @return dnp3h_st New var
 */
dnp3h_st mkHeader(uint8_t headerRawHex[]) {
    dnp3h_st header;

    memcpy(&header, headerRawHex, sizeof(header));

    return header;
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
        splitTwoByte(header.des, 0), splitTwoByte(header.des, 1),       // split these 2 byte ints
        splitTwoByte(header.src, 0), splitTwoByte(header.src, 1)        // into single bytes
    };

    // if already false, dont set back to true
    valid = header.crc == calculateCRC(checkHex, CRC_HEADER) && valid == 1 ? 1 : 0;

    return valid;
}

/** ~Asthetic~ Print. Just for testing. 
 * @param header The header to be decomposed and printed
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
 * @brief Extracts bit-by-bit information from DLC byte. 
 * 
 * @param header 
 */
void extractDLC(dnp3h_st header) {
    uint8_t dirBit;
    uint8_t prmBit;
    uint8_t fcbBit;
    uint8_t fcv_dfcBit; // either dfc or fcv depending on packet prm
    uint8_t fncCodeBits;    // 4 bits

    int response = 0;

    // 1. Determine direction
    dirBit = header.dlc & BIT_7_MASK;  // mask out bit 7
    if (dirBit != 0) {      // bit on
        printf("From Primary\n");
    }
    else {      // bit off
        printf("Secondary\n");
    }

    // 2. Determine primarity (initiation vs responding)
    prmBit = header.dlc & BIT_6_MASK;  // mask out bit 6
    if (prmBit != 0) {      // bit on
        printf("Primary to Secondary\n");
        response = 0;
    }
    else {      // bit off
        printf("Secondary to Primary\n");
        response = 1;
    }

    // 3. Examine FCV bit
    fcv_dfcBit = header.dlc & BIT_4_MASK;    // mask bit 4
    if(fcv_dfcBit != 0) {
        printf("Examine FCB Bit\n");
        // 3a. Check FCB Bit
        fcbBit = header.dlc & BIT_5_MASK;
        printf("FCB: %d\n", fcbBit != 0 ? 1 : 0);
    }
    else {
        printf("Ignore FCB Bit\n");
    }

    // 4. Check Function Code
    fncCodeBits = header.dlc & DLC_FUNCTION_CODE_MASK;
    if (response == 0) {
        printf("Primary Function Code: ");
        switch(fncCodeBits) {
            case DLC_RESET_LINK_STATES:
                printf("Rest Link States\n");
                break;
            case DLC_TEST_LINK_STATES:
                printf("Test Link States\n");
                break;
            case DLC_CONFIRMED_USER_DATA:
                printf("Confirmed User Data\n");
                break;
            case DLC_UNCONFIRMED_USER_DATA:
                printf("Unconfirmed User Data\n");
                break;
            case DLC_REQUEST_LINK_STATUS:
                printf("Request Link Status\n");
                break;
            default:
                printf("No Matching Code!\n");     // error: no matching code
        }
    }
    else {
        printf("Secondary Function Code: ");
        switch(fncCodeBits) {
            case DLC_ACK:
                printf("Acknowledge\n");
                break;
            case DLC_NACK:
                printf("Not Acknowledge\n");
                break;
            case DLC_LINK_STATUS:
                printf("Link Status\n");
                break;
            case DLC_NOT_SUPPORTED:
                printf("Not Supported\n");
                break;
            default:
                printf("No Matching Code!\n");     // error: no matching code
        }
    }
}