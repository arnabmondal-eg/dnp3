#include "freqPackets.h"
#include "crc.h"

static void _add_crc(uint8_t *stream, int extra_bytes) {
    uint8_t temp[16] = {0};
    uint16_t crc = 0;

    // save extra
    stream -= extra_bytes;
    memcpy(&temp, stream, extra_bytes);

    // calculate crc
    crc = calculateCRC(stream-16, 16);
    memcpy(stream, &crc, 2);
    printf("%04x ", crc);

    // add extra stuff back
    stream += 2;
    memcpy(stream, &temp, extra_bytes);
}

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

    request_sp->object_header_s.group = group >= 1 ? group : 1;
    request_sp->object_header_s.variation = variation > 0 ? variation : 1;
    request_sp->object_header_s.qualPrefix = 0;
    request_sp->object_header_s.qualRangeCode = 2;

    request_sp->object_header_s.rangeStart = data_start >= 0 ? data_start : 0;
    request_sp->object_header_s.rangeStop = data_stop > data_start ? data_stop : data_start+1;

    // add final crc
    send_buffer[24] = calculateCRC(&send_buffer[10], 16) >> 8;
    send_buffer[25] = calculateCRC(&send_buffer[10], 16);

    return 0;
}

/**
 * @brief Writes data
 * 
 * @param send_buffer 
 * @param recieve_buffer 
 * @param points Pass pointer to 8 byte array
 * @param total_points 
 * @return int 
 */
int write_data(uint8_t *send_buffer, uint8_t *recieve_buffer, uint64_t points[], int total_points) {
    dnp3_data_reply_st *packet_sp = send_buffer;
    
    header_st *recieve_header_sp = recieve_buffer;
    objectHeader_st *recieve_object_sp = recieve_buffer + 13;

    data0101_st *data0101_sp = {0};
    data0102_st *data0102_sp = {0};
    data3001_st *data3001_sp = {0};
    data3002_st *data3002_sp = {0};

    uint16_t crc = 0;
    int group = 0;
    int variation = 0;
    int extra_length = 0;

    int data_size = 0;
    int bytes_since_crc = 16;

    // find size of new packet
    group = recieve_object_sp->group;
    variation = recieve_object_sp->variation;
    switch(group) {
        case 1:
            if(variation == 1 || variation == 2) {
                data_size = 1;
            }
            else {
                return -1;
            }

            break;
        case 30:
            if(variation == 1) {
                data_size = 5;
            }
            else if(variation == 2) {
                data_size = 3;
            }
            else {
                return -1;
            }

            break;
        default:
            return -1;
            break;
    }

    extra_length = 2;
    if(group == 1 && variation == 1) {
        extra_length += total_points/8 + 1;
    }
    else {
        extra_length += total_points * data_size;
    }
    
    // packet header
    packet_sp->start = 0x6405;
    packet_sp->length = 19 + extra_length;
    
    // set dlc info
    packet_sp->dlc_s.dir = 0;
    packet_sp->dlc_s.prm = 0;
    packet_sp->dlc_s.fcb = 0;
    packet_sp->dlc_s.fcv_dfc = 0;
    packet_sp->dlc_s.function_code = 4;    // request user data

    // flip source and destination
    packet_sp->destination = recieve_header_sp->src;
    packet_sp->source = recieve_header_sp->des;

    // set crc
    packet_sp->header_crc = calculateCRC(send_buffer, 8);

    packet_sp->transport_header.final = 1;
    packet_sp->transport_header.first = 1;
    packet_sp->transport_header.segmentSequence = 0;

    packet_sp->application_header_s.acFirst = 1;
    packet_sp->application_header_s.acFinal = 1;
    packet_sp->application_header_s.acApplConfirm = 0;
    packet_sp->application_header_s.acUnsolicited = 0;
    packet_sp->application_header_s.acFragmentSequence = 0;
    packet_sp->application_header_s.applicationFunctionCode = 1;
    packet_sp->inn_s.inn1 = 0;
    packet_sp->inn_s.inn2 = 0;

    packet_sp->object_header_s.group = group >= 1 ? group : 1;
    packet_sp->object_header_s.variation = variation > 0 ? variation : 1;
    packet_sp->object_header_s.qualPrefix = 0;
    packet_sp->object_header_s.qualRangeCode = 2;
    packet_sp->object_header_s.rangeStart = recieve_object_sp->rangeStart;
    packet_sp->object_header_s.rangeStop = recieve_object_sp->rangeStop;

    send_buffer += sizeof(dnp3_data_reply_st);
    
    // write data
    switch(group) {
        case 1:
            if(variation == 1) {
                data0101_sp = (data0101_st *) points;
                for(int i = 0; i < (total_points / 8); i++) {
                    // check and add crc
                    if(bytes_since_crc >= 16) {
                        bytes_since_crc -= 16;
                        _add_crc(send_buffer, bytes_since_crc);
                        send_buffer += 2;
                    }

                    memcpy(send_buffer, data0101_sp, 1);    // copy data

                    points += 1;
                    data0101_sp = (data0101_st *) points;
                    send_buffer++;
                    bytes_since_crc++;
                }
            }
            else if(variation == 2) {
                data0102_sp = (data0102_st *) points;
                for(int i = 0; i < total_points; i++) {
                    // check and add crc
                    if(bytes_since_crc >= 16) {
                        bytes_since_crc -= 16;
                        _add_crc(send_buffer, bytes_since_crc);
                        send_buffer += 2;
                    }

                    memcpy(send_buffer, data0102_sp, 1);    // copy data

                    // next data point
                    points += 1;
                    data0102_sp = (data0102_st *) points;
                    send_buffer++;
                    bytes_since_crc++;
                }
            }
            break;
        case 30:
            if(variation == 1) {
                data3001_sp = (data3001_st *) points;
                for(int i = 0; i < total_points; i++) {
                    // check and add crc
                    if(bytes_since_crc >= 16) {
                        bytes_since_crc -= 16;
                        _add_crc(send_buffer, bytes_since_crc);
                        send_buffer += 2;
                    }

                    // copy point data into packet
                    memcpy(send_buffer, data3001_sp, 5);

                    send_buffer += 5;
                    points +=1;
                    data3001_sp = (data3001_st *) points;
                    bytes_since_crc += 5;
                }
            }
            else if(variation == 2) {
                data3002_sp = (data3002_st *) points;
                for(int i = 0; i < total_points; i++) {
                    // check and add crc
                    if(bytes_since_crc >= 16) {
                        bytes_since_crc -= 16;
                        _add_crc(send_buffer, bytes_since_crc);
                        send_buffer += 2;
                    }

                    // copy point data
                    memcpy(send_buffer, data3002_sp, 3);

                    send_buffer += 3;
                    points += 1;
                    data3002_sp = (data3002_st *) points;
                    bytes_since_crc += 3;
                }                
            }
            break;
    }

    // add crc
    crc = calculateCRC(send_buffer-16, 16);
    memcpy(send_buffer, &crc, 2);
    send_buffer +=2;
    return 0;
}