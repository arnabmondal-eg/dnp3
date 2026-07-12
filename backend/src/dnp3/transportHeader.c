#include "transportHeader.h"

transportHeader_st mkTransportHeader(uint8_t inputHex[], int *caretPosition) {
    transportHeader_st transportHeader_s;

    memcpy(&transportHeader_s, &inputHex[*caretPosition], 1);
    *caretPosition += 1;

    return transportHeader_s;
}

void printTransportHeader(transportHeader_st transportHeader_s) {
    printf("---- TRANSPORT HEADER ----\n");

    if (transportHeader_s.first != 0) printf("First Packet\n");
    else printf("Not First Packet\n");

    if(transportHeader_s.final != 0) printf("Final Packet\n");
    else printf("Not Final Packet\n");

    printf("Segment Sequence (Packet Number): %d\n", transportHeader_s.segmentSequence);
}
