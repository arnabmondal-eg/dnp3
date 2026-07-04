#include "header.h"
#include "packet.h"

header_st mkHeader(dnp3p_st *packet_s) {
    header_st header_s = {0};
    uint8_t hexInput[] = *packet_s -> hexInput;

    memcpy(&header_s, hexInput, sizeof(header_s));

    packet_s -> caretPosition += SIZEOFHEADER;

    return header_s;
}

int checkHeaderValidity(header_st header_s) {
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

void printHeader(header_st header_s) {
    printf("---- Packet Header ----\n");
    printf("Start: 0x%02X 0x%02X\n", header_s.s1, header_s.s2);
    printf("Length: 0x%02X [%d]\n", header_s.len, header_s.len);
    printf("Data Link Control: 0x%02X\n", header_s.dlc);
    printf("Destination: 0x%04X [%d]\n", header_s.des, header_s.des);
    printf("Source: 0x%04X [%d]\n", header_s.src, header_s.src);
    printf("CRC: 0x%04X\n", header_s.crc);
}