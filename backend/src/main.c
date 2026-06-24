#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// main functionality
// #include "packet.h"
#include "objectHeader.h"

//helpers
#include "crc.h"
#include "binaryHelper.h"

int main() {
    uint8_t request[] = {
        0x05, 0x64, 0x0D, 0xC4, 0xC8, 
        0x00, 0x01, 0x00, 0x6E, 0x78, 
        0xD9, 0xCA, 0x01, 0x1E, 0x02, 
        0x00, 0x01, 0x2F, 0x70, 0xA6
    };

    uint8_t reply[] = {
        0x05, 0x64, 0x77, 0x44, 0x01, 
        0x00, 0x49, 0x03, 0x67, 0x21,
        0xCC, 0xC9, 0x81, 0x00, 0x00,
        0x1e, 0x02, 0x01, 0x01, 0x00,
        0x22, 0x00, 0x01, 0x40, 0x5c,
        0x01, 0xfa, 0x56, 0xf8, 0x33,
        0x01, 0x98
    };

    // dnp3p_st request_s = mkPacket(request, sizeof(request));
    // dnp3p_st reply_s = mkPacket(reply, sizeof(reply));

    // freopen("log/log.txt", "w", stdout);   // that was increadibly easy

    // printf("-------- Request Packet --------\n");
    // printPacket(request_s);
    // printf("\n\n-------- Reply Packet --------\n");
    // printPacket(reply_s);

    dnp3objh_st objH_s = mkObjectHeader(reply);
    printObjectHeader(objH_s);

    
    return 0;
}