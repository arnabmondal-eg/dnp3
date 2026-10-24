#include "rawPacketHelper.h"

int getPacketSize(const uint8_t input[]) {
    int link_length = input[2];

    if (link_length < 5) {
        return -1;  // invalid length; callers must handle this
    }

    int payload_bytes = link_length - 5;
    int crc_blocks = (payload_bytes + 15) / 16;

    return 10 + payload_bytes + 2 * crc_blocks;
}
void printRawPacket(uint8_t packet[]) {
    const int rowLength = 16;

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