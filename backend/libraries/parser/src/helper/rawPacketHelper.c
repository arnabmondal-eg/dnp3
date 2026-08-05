#include "rawPacketHelper.h"

int getPacketSize(uint8_t input[]) {
    int decSize = 0;
    int trueSize = 0;
    int numberOfCRC = 0;

    decSize = input[2];
    numberOfCRC = (decSize - 5) / 16;
    if (decSize > 5) trueSize = (numberOfCRC * 18) + 10 + ((decSize - 5) % 16) + 2;
    else trueSize = 10;

    // each crc "block" has 2 crc bytes, 10 from header, any left over

    return trueSize;
}

void printRawPacket(uint8_t packet[]) {
    const int rowLength = 10;

    int length = 0;
    int extra = 0;
    int cntr = 0;

    // get packet length
    length = getPacketSize(packet);

    // extra
    extra = length % rowLength;

    // print packet
    for(int i = 0; i < length / rowLength; i++) {
        for(int j = 0; j < rowLength; j++) {
            printf("%02X ", packet[cntr++]);
        }
        printf("\n");
    }
    for(int i = 0; i < extra; i++) {
        printf("%02X ", packet[cntr++]);
    }

    printf("\n");
}