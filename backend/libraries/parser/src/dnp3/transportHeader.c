#include "transportHeader.h"

transport_header_st transport_header(uint8_t inputHex[], int *caretPosition) {
    transport_header_st transport_header_s;

    memcpy(&transport_header_s, &inputHex[*caretPosition], 1);
    *caretPosition += 1;

    return transport_header_s;
}

void print_transport_header(transport_header_st transport_header_s) {
    printf("---- Transport Header ----\n");

    if (transport_header_s.first != 0) printf("First Packet\n");
    else printf("Not First Packet\n");

    if(transport_header_s.final != 0) printf("Final Packet\n");
    else printf("Not Final Packet\n");

    printf("Segment Sequence (Packet Number): %d\n", transport_header_s.segmentSequence);
}
