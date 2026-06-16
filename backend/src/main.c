#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// main functionality
#include "mainHeader.h"
#include "dlc.h"
#include "transportHeader.h"
#include "applicationHeader.h"

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

    dnp3h_st header_s = mkHeader(reply);
    dnp3hDLC_st headerDLC_s = mkDLC(header_s);
    dnp3th_st thHeader_s = mkTransportHeader(reply, sizeof(reply));
    dnp3aph_st applHeader_s = mkApplicationHeader(reply);
    
    printHeader(header_s);
    printDLCData(headerDLC_s);
    printTransportHeader(thHeader_s);
    printApplicationHeader(applHeader_s);
    
    return 0;
}