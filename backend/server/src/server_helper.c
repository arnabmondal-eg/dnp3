#include "server_helper.h"

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

int server_interpret(uint8_t *recieve_buffer, uint8_t  *send_buffer, int total_points, uint64_t *points) {
    header_st *header_sp = {0};
    header_st ack_s = {0};

    header_sp = (header_st *) recieve_buffer;   // map recieve to header struct
    

    printf("Func Code: %d\n", header_sp->dlc_s.function_code);
    switch (header_sp->dlc_s.function_code) {

        case 0:
            log_info(INFO_BOTH, "Recieved request to reset link\n");

            ack_s = dnp3Lib_mkAck(
                !(header_sp->dlc_s.dir),      // reverse the dlc bit
                header_sp->src,         // switch src with des
                header_sp->des          // switch des with src
            );
            memcpy(send_buffer, &ack_s, 10);
            break;

        case 3:
        case 4:
            log_info(INFO_BOTH, "Recived request for server data\n");
            write_data(send_buffer, recieve_buffer, points, total_points);      // fill send buffer with response
            break;
        
        default:
            errno = 22;
            log_warn(errno, "interpret_packet");

            break;
    }

    return 0;
}