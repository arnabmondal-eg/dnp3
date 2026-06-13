#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// main functionality
#include "header.h"
#include "dlc.h"

//helpers
#include "crc.h"
#include "binaryHelper.h"

int main() {
    uint8_t test_header[] = {0x05, 0x64, 0x0D, 0xC4, 0x49, 0x03, 0x01, 0x00, 0x92, 0x9C};

    dnp3h_st header_s = mkHeader(test_header);
    dnp3hDLC_st headerDLC_s = mkDLC(header_s);
    
    printDLCData(headerDLC_s);
    
    return 0;
}