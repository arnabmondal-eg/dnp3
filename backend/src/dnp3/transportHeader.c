#include "transportHeader.h"

transportHeader_st mkTransportHeader(dnp3p_st *packet_s) {
    transportHeader_st transportHeader_s;
    uint8_t *hexInput[] = packet_s -> hexInput;
    // if(length <= 10) {
    //     if (DEBUG) printf("Provided Input Too Small! Transport Header Returned Uninitilized!\n");
    //     return transportHeader_s;
    // }

    memcpy(&transportHeader_s, &hexInput[TRANSPORT_HEADER_POSTION], 1);

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

//TODO: Write getTransportHeader bit function