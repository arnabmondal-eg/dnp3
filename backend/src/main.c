#include <stdio.h>
#include <stdbool.h>
#include "header/header.h"

extern bool checkHeaderValidity(char headerRawHex[]);
extern u_int16_t calculateCRC(const u_int8_t data[], size_t length);

int main() {
    char test_header[] = {0x05, 0x64, 0x0D, 0xC4, 0x49, 0x03, 0x01, 0x00, 0x92, 0x9C};

    printf("Calculated CRC Value: 0x%04X\n", calculateCRC(test_header, 8));

    printf("Packet Validty: %s\n", checkHeaderValidity(test_header) ? "true" : "false");

    return 0;
}