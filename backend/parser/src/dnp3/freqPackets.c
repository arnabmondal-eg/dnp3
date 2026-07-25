#include "freqPackets.h"
#include "crc.h"

dnp3Lib_resetLinkP_st dnp3Lib_mkResetLink(int destination, int source) {
    dnp3Lib_resetLinkP_st packet_s = {0};
    uint8_t packetArr[8] = {0};

    packet_s.start = 0x0564;
    packet_s.length = 5;
    packet_s.dlc = 0xC0;

    packet_s.destination = (uint16_t) destination;
    packet_s.source = (uint16_t) source;

    memcpy(&packetArr, &packet_s, 8);
    
    packet_s.crc = calculateCRC(packetArr, 8);

    return packet_s;
}

dnp3Lib_ackP_st dnp3Lib_mkAck(int direction, int destination, int source) {
    dnp3Lib_ackP_st packet_s = {0};
    uint8_t packetArr[8] = {0};

    packet_s.start = 0x0564;
    packet_s.length = 5;
    packet_s.dlc = direction == 1 ? 0x80 : 0x00;    // master: 80, remote 00

    packet_s.destination = (uint16_t) destination;
    packet_s.source = (uint16_t) source;

    memcpy(&packetArr, &packet_s, 8);
    
    packet_s.crc = calculateCRC(packetArr, 8);

    return packet_s;
}

dnp3Lib_ackP_st dnp3Lib_mkNack(int direction, int destination, int source) {
    dnp3Lib_ackP_st packet_s = {0};
    uint8_t packetArr[8] = {0};

    packet_s.start = 0x0564;
    packet_s.length = 5;
    packet_s.dlc = direction == 1 ? 0x81 : 0x01;    // master: 80, remote 00

    packet_s.destination = (uint16_t) destination;
    packet_s.source = (uint16_t) source;

    memcpy(&packetArr, &packet_s, 8);
    
    packet_s.crc = calculateCRC(packetArr, 8);

    return packet_s;
}