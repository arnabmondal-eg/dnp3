#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "header.h"

bool checkPacketValidity(char headerRawHex[11]) {
    bool vaild = false;     // assume invalid
    dnp3h_st header;

    memcpy(&header, headerRawHex, sizeof(header));   // map chars to header struct

    if (header.s1 == 0x05 && header.s2 == 0x64) {
        vaild = true;
        printf("Packet Vaild\n");
    }
    else {
        vaild = false;
        printf("Packet Invaild\nStart Byte 1 Returned %d rather than 0x05\nStart Byte 2 Returned %d rather than 0x64", header.s1, header.s2);
    }


    // Temp Printers
    printf("Start 1: %d, Start 2: %d\n", header.s1, header.s2);
    printf("Destination: %d, Source: %d\n", header.des, header.src);
    return vaild;
}