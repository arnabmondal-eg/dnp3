#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "header/header.h"
#include "helper/crc.h"

int main() {
    char test_header[] = {0x05, 0x64, 0x0D, 0xC4, 0x49, 0x03, 0x01, 0x00, 0x92, 0x9C};

    dnp3h_st header = mkHeader(test_header);

    printHeader(header);

    return 0;
}