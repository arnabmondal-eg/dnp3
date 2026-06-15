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
        0x05, 0x64, 0x0A, 0xC4, 0x01, 0x00, 0x00, 0x00, 0xF0, 
        0xB8, 0xC0, 0x01, 0x3C, 0x02, 0x06, 0x5E, 0x9A
    };

    uint8_t reponse[] = {
        0x05, 0x64, 0x14, 0x44, 0x00, 0x00, 0x01, 0x00, 0xA1, 
        0xC7, 0xC0, 0x81, 0x01, 0x02, 0x01, 0x01, 0x7B, 0xE4, 
        0x1E, 0x01, 0x01, 0x2A, 0x00, 0x00, 0x00, 0x6C, 0x91
    };

    dnp3h_st header_s = mkHeader(request);
    dnp3hDLC_st headerDLC_s = mkDLC(header_s);
    dnp3th_st thHeader_s = mkTransportHeader(request, sizeof(request));
    dnp3aph_st applHeader_s = mkApplicationHeader(request);
    
    printHeader(header_s);
    printDLCData(headerDLC_s);
    printTransportHeader(thHeader_s);
    printApplicationHeader(applHeader_s);
    
    return 0;
}