#include "packet.h"


dnp3p_st mkPacketAuto(uint8_t input[]) {
    dnp3p_st packet_s = {0};

    packet_s.hexInput = input;

    packet_s.header_s = header(input, &packet_s.caretPosition);
    //mkHeader1(hexInput, &packet_s);
    packet_s.transport_header_s = transport_header(input, &packet_s.caretPosition);
    packet_s.application_header_s = application_header(input, &packet_s.inn_s, &packet_s.caretPosition, &packet_s.header_s.dlc_s, &packet_s.innActive);
    packet_s.object_header_s = mkObjectHeader(input, &packet_s.caretPosition);
    if(packet_s.object_header_s.qualRangeCode <= 3) {
        packet_s.total_points = packet_s.object_header_s.rangeStop - packet_s.object_header_s.rangeStart + 1;
    }
    else {
        packet_s.total_points = packet_s.object_header_s.rangeStart;
        
        packet_s.object_header_s.rangeStart = 0;
        packet_s.object_header_s.rangeStop = 0;
    }

    return packet_s;
}

dnp3p_st dnp3lib_mkpacket_manual(int destination, int source, int group, int variation, int startIndex, int stopIndex) {
    dnp3p_st packet_s = {0};

    /* Passed In Defintions */
    packet_s.header_s.des = destination;
    packet_s.header_s.src = source;

    packet_s.object_header_s.group = group;
    packet_s.object_header_s.variation = variation;

    if(stopIndex == 0) {
        packet_s.object_header_s.rangeStart = 0;
        packet_s.object_header_s.rangeStop = 0;

        packet_s.total_points = (stopIndex - startIndex + 1);
    }
    else {
        packet_s.object_header_s.rangeStart = startIndex;
        packet_s.object_header_s.rangeStop = stopIndex;
    }

    /* Calculated Defintions */

    // object header
    if(packet_s.object_header_s.rangeStart == 0 && packet_s.object_header_s.rangeStart == 0) {
        // all points of a given type
        if (startIndex == 0) {
            packet_s.object_header_s.qualRangeCode = 6;
        }
        // 1 octect quantity
        else if(startIndex > 0 && startIndex < UINT8_MAX) {
            packet_s.object_header_s.qualRangeCode = 7;
        }
        // 2 octect quantity
        else if(startIndex > UINT8_MAX && startIndex < UINT16_MAX) {
            packet_s.object_header_s.qualRangeCode = 8;
        }
        // 4 octect quantity
        else if(startIndex > UINT16_MAX && startIndex < UINT32_MAX) {
            packet_s.object_header_s.qualRangeCode = 9;
        }
        else {
            errno = 0x4b;   // EOVERFLOW 75 Value too large for defined data type
            perror("create packet");
        }
    }
    else {
        int checkInt = (int) stopIndex;
        if(startIndex > stopIndex) {
            checkInt = startIndex;
        }

        // 1 octect start and stop indices (total range size = 2)
        if(checkInt > 0 && checkInt < 2 * UINT8_MAX) {
            packet_s.object_header_s.qualRangeCode = 0;
        }
        // 2 octect start and stop indices (total range size = 4)
        else if(checkInt > 2 * UINT8_MAX && checkInt < 2 * UINT16_MAX) {
            packet_s.object_header_s.qualRangeCode = 1;
        }
        // 4 octect start and stop
        else if(checkInt > 2 * UINT16_MAX && checkInt < 2 * UINT32_MAX) {
            packet_s.object_header_s.qualRangeCode = 2;
        }
        else {
            errno = 0x4b;   // EOVERFLOW 75 Value too large for defined data type
            perror("create packet");
        }
    }

    //TODO: qualPrefix
}

void printPacket(dnp3p_st packet_s) {
    printHeader(packet_s.header_s);
    printDLCData(packet_s.header_s.dlc_s);
    print_transport_header(packet_s.transport_header_s);
    print_application_header(packet_s.application_header_s, packet_s.inn_s, packet_s.innActive);
    printObjectHeader(packet_s.object_header_s);
    if(packet_s.header_s.dlc_s.dir != 1) printData(packet_s.hexInput, &packet_s.caretPosition, &packet_s.object_header_s, packet_s.total_points);

    // printf("Data 1: %d", ((data0101_st*)packet_s.data_s)->data);

    // printf("\nValidity: %d\n", packet_s.packetValidity);
}