#include "packet.h"

dnp3p_st mkPacket(uint8_t input[]) {
    dnp3p_st packet_s = {0};

    packet_s.hexInput = input;


    packet_s.header_s = mkHeader(input, &packet_s.caretPosition);
    //mkHeader1(hexInput, &packet_s);
    packet_s.dlc_s = mkDLC(input, &packet_s.caretPosition);
    packet_s.transportHeader_s = mkTransportHeader(input, &packet_s.caretPosition);
    packet_s.applicationHeader_s = mkApplicationHeader(input, &packet_s.caretPosition, &packet_s.dlc_s);
    packet_s.objectHeader_s = mkObjectHeader(input, &packet_s.caretPosition);

    return packet_s;
}

void printPacket(dnp3p_st packet_s) {
    printHeader(packet_s.header_s);
    printDLCData(packet_s.dlc_s);
    printTransportHeader(packet_s.transportHeader_s);
    printApplicationHeader(packet_s.applicationHeader_s);
    printObjectHeader(packet_s.objectHeader_s);
    if(packet_s.dlc_s.dirBit != 1) printData(packet_s.hexInput, &packet_s.caretPosition, &packet_s.objectHeader_s);

    // printf("Data 1: %d", ((data0101_st*)packet_s.data_s)->data);

    // printf("\nValidity: %d\n", packet_s.packetValidity);
}