#include "transportHeader.h"

dnp3th_st mkTransportHeader(uint8_t inputHex[], int length) {
    dnp3th_st transportHeader_s;
    if(length <= 10) {
        if (DEBUG) printf("Provided Input Too Small! Transport Header Returned Uninitilized!\n");
        return transportHeader_s;
    }
    
    transportHeader_s.first = inputHex[TRANSPORT_HEADER_POSTION] & BIT_7_MASK != 0 ? 1 : 0;
    transportHeader_s.final = inputHex[TRANSPORT_HEADER_POSTION] & BIT_6_MASK != 0 ? 1 : 0;

    // mask out first 2 bits
    transportHeader_s.segmentSequence = inputHex[TRANSPORT_HEADER_POSTION] & 0x3F;     // 0011 1111

    return transportHeader_s;
}

void printTransportHeader(dnp3th_st transportHeader_s) {
    printf("\n---- TRANSPORT HEADER ----\n");

    if (transportHeader_s.first != 0) {
        printf("First Packet\n");
    }
    else {
        printf("Not First Packet\n");
    }

    if(transportHeader_s.final != 0) {
        printf("Final Packet\n");
    }
    else {
        printf("Not Final Packet\n");
    }

    printf("Segment Sequence (Packet Number): %d\n\n", transportHeader_s.segmentSequence);
}

//TODO: Write getTransportHeader bit function