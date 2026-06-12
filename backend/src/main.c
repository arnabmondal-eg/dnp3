#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "header/header.h"
#include "helper/crcHelper/crc.h"
#include "helper/binaryHelper/binaryHelper.h"

int main() {
    uint8_t test_header[] = {0x05, 0x64, 0x0D, 0xC4, 0x49, 0x03, 0x01, 0x00, 0x92, 0x9C};

    dnp3h_st header = mkHeader(test_header);
    extractDLC(header);

    return 0;
}