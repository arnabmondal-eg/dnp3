#include "freqPackets.h"
#include "crc.h"

dnp3p_st dnp3Lib_setNavigation(dnp3p_st packet_s, int destination, int source, int direction) {
    packet_s.header_s.dlc_s.dir = direction;
    
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
    packet_s.dlc_s = *(dlc_st *) 0xC0;

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
    packet_s.dlc_s = *(dlc_st *) (direction == 1 ? 0x80 : 0x00);    // master: 80, remote 00

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
    packet_s.dlc_s = *(dlc_st *) (direction == 1 ? 0x81 : 0x01);    // master: 80, remote 00

    packet_s.des = (uint16_t) destination;
    packet_s.src = (uint16_t) source;

    memcpy(&packetArr, &packet_s, 8);
    
    packet_s.crc = calculateCRC(packetArr, 8);

    return packet_s;
}

/**
 * @brief Creates a request for data based on passed params
 * 
 * @param send_buffer Buffer to write to
 * @param group Data type
 * @param variation Type variation
 * @param data_start Start index of data points
 * @param data_stop Stop index of data points
 * @param source Source
 * @param destination Destination
 * @return int -1 if error, 0 else
 */
int request_data(uint8_t *send_buffer, int group, int variation, int data_start, int data_stop, int source, int destination) {
    dnp3_data_request_st *request_sp;

    request_sp = send_buffer;       // write directly to buffer

    request_sp->start = 0x6405;
    request_sp->length = 19;
    
    // set dlc info
    request_sp->dlc_s.dir = 1;
    request_sp->dlc_s.prm = 1;
    request_sp->dlc_s.fcb = 0;
    request_sp->dlc_s.fcv_dfc = 0;
    request_sp->dlc_s.function_code = 4;    // request user data

    request_sp->source = source >= 0 ? source : 0;  // if >= 0, source, else 0
    request_sp->destination = destination >= 0 ? destination : source+1;    // if greater than 0, des, else source+1

    request_sp->header_crc = calculateCRC(send_buffer, 8);

    request_sp->transport_header.final = 1;
    request_sp->transport_header.first = 1;
    request_sp->transport_header.segmentSequence = 0;

    request_sp->application_header_s.acFirst = 1;
    request_sp->application_header_s.acFinal = 1;
    request_sp->application_header_s.acApplConfirm = 0;
    request_sp->application_header_s.acUnsolicited = 0;
    request_sp->application_header_s.acFragmentSequence = 0;
    request_sp->application_header_s.applicationFunctionCode = 1;
    request_sp = (dnp3_data_request_st *)((uint8_t *)request_sp - 3);   // padding solution

    request_sp->object_header_s.group = group >= 1 ? group : 1;
    request_sp->object_header_s.variation = variation > 0 ? variation : 1;
    request_sp->object_header_s.qualPrefix = 0;
    request_sp->object_header_s.qualRangeCode = 2;

    request_sp = (dnp3_data_request_st *)((uint8_t *)request_sp - 1);   // bandaid solution for issues with padding
    request_sp->object_header_s.rangeStart = data_start >= 0 ? data_start : 0;
    request_sp->object_header_s.rangeStop = data_stop > data_start ? data_stop : data_start+1;

    // add final crc
    send_buffer[24] = calculateCRC(&send_buffer[10], 16) >> 8;
    send_buffer[25] = calculateCRC(&send_buffer[10], 16);

    return 0;
}