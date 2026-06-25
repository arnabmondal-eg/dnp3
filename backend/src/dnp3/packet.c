#include "packet.h"

dnp3p_st mkPacket(uint8_t hexInput[], int packetSize) {
    dnp3p_st packet_s;

    packet_s.hexInput = hexInput;

    packet_s.header_s = mkHeader(hexInput);
    packet_s.dlc_s = mkDLC(hexInput);
    packet_s.transportHeader_s = mkTransportHeader(hexInput, packetSize);
    packet_s.applicationHeader_s = mkApplicationHeader(hexInput);
    packet_s.objectHeader_s = mkObjectHeader(hexInput);

    return packet_s;
}

void printPacket(dnp3p_st packet_s) {
    printHeader(packet_s.header_s);
    printDLCData(packet_s.dlc_s);
    printTransportHeader(packet_s.transportHeader_s);
    printApplicationHeader(packet_s.applicationHeader_s);
    printObjectHeader(packet_s.objectHeader_s);
}