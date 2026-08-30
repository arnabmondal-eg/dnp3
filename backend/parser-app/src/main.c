#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// main functionality
#include "packet.h"
#include "freqPackets.h"

//helpers
#include "crc.h"
#include "binaryHelper.h"


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "log.h"

/**
 * @brief Creates an array of data, each index in the array id
 * 
 * @param group Supports either 1 or 30
 * @param varriation Either 1 or 2
 * @param total_points Total points to generate
 * @param points Array to fill, must be uint64
 */
void refresh_data(int group, int varriation, int total_points, uint64_t *points) {
    const uint8_t FLAGS = 0x80;
    
    int counter = 0;

    uint32_t value = 0;
    uint32_t min = 0;
    uint32_t max = 0;

    uint8_t *point_writer = points;

    srand(time(NULL));

    if(group == 1) {
        max = 1;
    }
    else if(group == 30) {
        if(varriation == 1) {
            max = UINT32_MAX;
        }
        else if(varriation == 2) {
            max = UINT16_MAX;
        }
    }
    else return;


    for(int i = 1; i <= total_points; i++) {
        if(group == 30) {
            // generate value
            value = (uint32_t) (rand() % (max - min)) + min;
            
            *point_writer++ = FLAGS;

            memcpy(point_writer, &value, 4);
            point_writer += 7;    // move to next space
        }
        else if(group == 1) {

            if (varriation == 2) {
                // generate value
                value = rand() % 2;
    
                ((data0102_st *) point_writer)->online = 1;
                ((data0102_st *) point_writer)->state = value;
            }
            else if(varriation == 1) {
                
                /* 
                Increidle lazy move by me
                I am aware
                I could have done it better
                I dont really care

                Wait why does that rhyme
                They should call me thyme
                The way Im writing these lines
                All of the time (yo im genuinly the goat)
                */

                value = rand() % 256;   // acutally a kinda terrible generator, but idc
                memcpy(point_writer, &value, 1);
            }

            // go to next cell
            point_writer += 8;
        }
    }
}


int main() {
    /*
    uint8_t request[] = {
        0x05, 0x64, 0x0D, 0xC4, 0xC8, 
        0x00, 0x01, 0x00, 0x6E, 0x78, 
        0xD9, 0xCA, 0x01, 0x1E, 0x02, 
        0x00, 0x01, 0x22, 0x70, 0xA6
    };

    uint8_t reply[] = {
        0x05, 0x64, 0x55, 0x44, 0x01,
        0x00, 0x49, 0x03, 0x67, 0x21,
        0xCC, 0xC9, 0x81, 0x00, 0x00,
        0x1e, 0x02, 0x01, 0x01, 0x00,
        0x22, 0x00, 0x01, 0x40, 0x5c,
        0x01, 0xfa, 0x56, 0x88, 0x33,
        0x01, 0xA8, 0x39, 0x01, 0x28,
        0x5c, 0x01, 0xA0, 0x19, 0x01,
        0xD8, 0x19, 0x01, 0x00, 0x43,
        0x30, 0x00, 0x01, 0x70, 0x00,
        0x01, 0xA8, 0x00, 0x01, 0x12,
        0x00, 0x01, 0x00, 0x06, 0x01,
        0x84, 0x00, 0xC6, 0x02, 0x01,
        0xA1, 0x00, 0x01, 0x00, 0x00,
        0x01, 0x16, 0x00, 0x01, 0x14,
        0x00, 0x01, 0x80, 0x06, 0x01,
        0xC5, 0xA2, 0x83, 0x74, 0x01,
        0x20, 0x18, 0x01, 0x97, 0x48,
        0x01, 0x6D, 0x79, 0x01, 0x56,
        0x77, 0x01, 0xCC, 0xFA, 0x16,
        0x07, 0x87
    };

    uint8_t cleanedRequest[getPacketSize(request)];
    removeCRC(request, cleanedRequest);

    uint8_t cleanedReply[getPacketSize(reply)];
    removeCRC(reply, cleanedReply);

    dnp3p_st request_s = mkPacketAuto(cleanedRequest);
    dnp3p_st reply_s = mkPacketAuto(cleanedReply);

    if(freopen("log/log.txt", "w", stdout) == NULL) {

        printf("Error Occurred while writing to file\nCreate log/ folder if missing");
        return 0;
    }

    printf("-------- Request Packet --------\n");
    printPacket(request_s);
    printf("\n\n-------- Reply Packet --------\n");
    printPacket(reply_s);
    */
    
    uint8_t request[192] = {0};
    uint8_t reply[192] = {0};
    uint64_t points[16] = {0};

    dnp3p_st request_s = {0};
    dnp3p_st reply_s = {0};


    request_data(request, 1, 1, 0, 16, 0, 1);
    printRawPacket(request);
    request_s = mkPacketAuto(request);
    printf("\n");
    
    refresh_data(1, 1, 16, points);
    for(int i = 1; i <= 16; i++) {
        printf("%lu ", points[i-1]);
        if(i % 4 == 0) printf("\n");
    }
    printf("\n");

    write_data(reply, request, points, 16);
    printRawPacket(reply);
    reply_s = mkPacketAuto(reply);

    // printf("\n\n");
    // printPacket(request_s);
    // printPacket(reply_s);


    return 0;
}