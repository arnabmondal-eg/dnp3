#include "freqPackets.h"
#include "crc.h"

dnp3p_st dnp3Lib_setNavigation(dnp3p_st packet_s, int destination, int source, int direction) {
    packet_s.dlc_s.dirBit = direction;
    
    packet_s.header_s.des = destination;
    packet_s.header_s.src = source;

    return packet_s;
}

header_st dnp3Lib_mkResetLink(int destination, int source) {
    header_st packet_s = {0};
    uint8_t packetArr[8] = {0};

    packet_s.s1 = 0x05;
    packet_s.s2 = 0x64;

    packet_s.len = 5;
    packet_s.dlc = 0xC0;

    packet_s.des = (uint16_t) destination;
    packet_s.src = (uint16_t) source;

    memcpy(&packetArr, &packet_s, 8);
    
    packet_s.crc = calculateCRC(packetArr, 8);

    return packet_s;
}

header_st dnp3Lib_mkAck(int direction, int destination, int source) {
    header_st packet_s = {0};
    uint8_t packetArr[8] = {0};

    packet_s.s1 = 0x05;
    packet_s.s2 = 0x64;

    packet_s.len = 5;
    packet_s.dlc = direction == 1 ? 0x80 : 0x00;    // master: 80, remote 00

    packet_s.des = (uint16_t) destination;
    packet_s.src = (uint16_t) source;

    memcpy(&packetArr, &packet_s, 8);
    
    packet_s.crc = calculateCRC(packetArr, 8);

    return packet_s;
}

header_st dnp3Lib_mkNack(int direction, int destination, int source) {
    header_st packet_s = {0};
    uint8_t packetArr[8] = {0};

    packet_s.s1 = 0x05;
    packet_s.s2 = 0x64;
    packet_s.len = 5;
    packet_s.dlc = direction == 1 ? 0x81 : 0x01;    // master: 80, remote 00

    packet_s.des = (uint16_t) destination;
    packet_s.src = (uint16_t) source;

    memcpy(&packetArr, &packet_s, 8);
    
    packet_s.crc = calculateCRC(packetArr, 8);

    return packet_s;
}

dnp3p_st dnp3Lib_mkAnalogReadRequest(int destination, int source, int start, int stop, int variation) {
    dnp3p_st packet_s = {0};
    int indexSize = 0;

    packet_s.header_s.des = destination;
    packet_s.header_s.src = source;

    packet_s.objectHeader_s.group = 30;     // continious
    packet_s.objectHeader_s.variation = variation;
}