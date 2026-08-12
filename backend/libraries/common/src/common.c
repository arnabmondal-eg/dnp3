#include "common.h"

#define DEBUG 0

#define LOGSTR_SENDPACKET "common/send_packet"
#define LOGSTR_RECIEVEPACKET "common/recieve_packet"
#define LOGSTR_INTERPRETPACKET "common/interpret_packet"
#define LOGSTR_POLLCONNECTIONS "common/poll_connections"

/**
 * @brief Setsup connections to server
 * 
 * @param num_connections Total number of connections
 * @param connections Array contining socket for each connection
 * @param ufds pollfd array to fill
 * @return int Returns -1 if error, 0 else
 */
int init_connection(int num_connections, struct pollfd ufds[]) {
    if(num_connections > COMMON_MAX_CONNECTIONS) {
        errno = 87;
        log_warn(errno, "common/init_connection");
        log_info(INFO_BOTH, "Edit 'common.h' to increase maximum connections amount\n");

        num_connections = COMMON_MAX_CONNECTIONS;
    }

    log_info(INFO_BOTH, "Creating %d Connections\n", num_connections);

    for(int i=0; i < num_connections; i++) {
        ufds[i].events = POLLIN;
    }

    return 0;
}

/**
 * @brief Sends a packet to specificed connection
 * 
 * @param connection_socket Socket of Connection
 * @param send_buffer Buffer containing message
 * @return int Returns -1 if Error, 0 Else
 */
int send_packet(const int connection_socket, const uint8_t send_buffer[]) {
    int send_status = 0;

    log_info(INFO_CONSOLE, "Sending Packet\n");

    send_status = send(connection_socket, send_buffer, getPacketSize(send_buffer), 0);
    if(send_status == -1) {
        log_warn(errno, "common/send_packet");
        return -1;
    }

    log_info(INFO_BOTH, "Sent %d Bytes\n", getPacketSize(send_buffer));
    
    return 0;
}

/**
 * @brief Recived packet from specified connection
 * 
 * @param connection_socket Socket of Connection
 * @param recieve_buffer Buffer for sent packet
 * @return int Returns -1 if Error, 0 Else
 */
int recieve_packet(const int connection_socket, uint8_t recieve_buffer[]) {  
    int recive_status = 0;
    int packet_length = 0;
    int total_recieve = 0;
    
    recive_status = (int) recv(connection_socket, &recieve_buffer[0], 10, 0);
    if(recive_status == -1) {
        log_err(errno, LOGSTR_RECIEVEPACKET);
        return -1;
    }
    else if(recive_status == 0) {
        errno  = 61;
        log_warn(errno, LOGSTR_RECIEVEPACKET);

        return -1;
    }

    total_recieve = recive_status;

    packet_length = getPacketSize(recieve_buffer);
    if(packet_length == 10) {
        log_info(INFO_CONSOLE, "Recieved %d Bytes\n", total_recieve);
        return 0;
    }
    else {
        recive_status = (int) recv(connection_socket, &recieve_buffer[10], packet_length-10, 0);
    }

    if(recive_status == -1) {
        log_err(errno, LOGSTR_RECIEVEPACKET);
        return -1;
    }

    total_recieve += recive_status;
    log_info(INFO_CONSOLE, "Recieved %d Bytes\n", total_recieve);

    return 0;
}

/**
 * @brief Interprets Packet and Creates response (or hands off to other function)
 * 
 * @param recieve_buffer Packet to Interpret
 * @param send_buffer Created Packet
 * @return int Returns -1 if Error, 0 Else
 */
int interpret_packet(uint8_t recieve_buffer[], uint8_t send_buffer[]) {
    header_st *header_sp = {0};
    dlc_st *dlc_sp = {0};
    header_st ack_s = {0};

    header_sp = (header_st *) recieve_buffer;   // map recieve to header struct
    dlc_sp = (dlc_st *)&(header_sp->dlc);
    

    switch (header_sp->dlc) {

        case 0xC0:
            {

            ack_s = dnp3Lib_mkAck(
                !(dlc_sp->dirBit),      // reverse the dlc bit
                header_sp->src,         // switch src with des
                header_sp->des          // switch des with src
            );
            memcpy(send_buffer, &ack_s, 10);
            }
            break;
        
        default:
            errno = 22;
            log_warn(errno, LOGSTR_INTERPRETPACKET);

            break;
    }

    return 0;
}

/**
 * @brief Polls connections in ufds for data
 * 
 * @param ufds Array containing connections
 * @param total_connections Total Connections in ufds
 * @param timeout Time to wait
 * @return int -1 on poll error, Number of connections with data else
 */
int poll_connections(struct pollfd ufds[], int total_connections, const int timeout) {
    int poll_status;
    
    poll_status = poll(ufds, total_connections, timeout < -1 ? 0 : timeout);    // -1 timeout is infinite

    if(poll_status == -1) {
        log_warn(errno, LOGSTR_POLLCONNECTIONS);
        return -1;
    }
    else if(poll_status == 0) {
        errno = 61; // no data availible
        log_warn(errno, LOGSTR_POLLCONNECTIONS);
    }

    log_info(INFO_BOTH, "Polled %d connections, %d have data\n", total_connections, poll_status);

    return poll_status;
}