#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// main functionality
#include "packet.h"

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
        0x05, 0x64, 0x9e, 0x44, 0x01, 
        0x00, 0xc8, 0x00, 0xa3, 0xaa, 
        0xf0, 0xca, 0x81, 0x87, 0x80, 
        0x1e, 0x02, 0x01, 0x01, 0x00, 
        0x2f, 0x00, 0x01, 0xf8, 0x5c, 
        0x01    // ill be adding more as i add more func. to the program
    };

    dnp3p_st request_s = mkPacket(request, sizeof(request));
    dnp3p_st reply_s = mkPacket(reply, sizeof(reply));

    printf("\n\n-------- Request Packet --------\n\n");
    printPacket(request_s);
    printf("\n\n-------- Reply Packet --------\n\n");
    printPacket(reply_s);
    
    return 0;
}