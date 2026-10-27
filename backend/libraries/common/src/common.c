#include "common.h"

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

    log_info(INFO_BOTH, "Sending Packet\n");

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
    int desired_recieveamnt = 0;
    int total_recieve = 0;

    int recive_status = 0;
    int packet_length = 0;
    
    // get packet header
    desired_recieveamnt = 10;
    while(total_recieve < 10) {
        recive_status = (int) recv(connection_socket, recieve_buffer+total_recieve, 10-total_recieve, 0);
        if(recive_status == -1) {
            log_warn(errno, LOGSTR_RECIEVEPACKET);
            return -1;
        }
        else if(recive_status == 0) {
            errno  = 61;
            log_warn(errno, LOGSTR_RECIEVEPACKET);
    
            return -1;
        }
        else total_recieve += recive_status;
    }

    // get packet length
    packet_length = getPacketSize(recieve_buffer);
    if(packet_length == 10) {
        log_info(INFO_BOTH, "Recieved %d Bytes\n", total_recieve);
        return 0;
    }
    else {
        desired_recieveamnt = packet_length;
        while(total_recieve < desired_recieveamnt) {
            recive_status = (int) recv(connection_socket, &recieve_buffer[total_recieve-1], packet_length-total_recieve, 0);
            if(recive_status > 0) {
                total_recieve += recive_status;
            }
            else if(recive_status <= 0) {
                if(errno = EINTR) {
                    // retry
                    errno = 0;
                }
                else {
                    log_warn(errno, LOGSTR_RECIEVEPACKET);
                    return -1;
                }
            }
        }
    }

    log_info(INFO_BOTH, "Recieved %d Bytes\n", total_recieve);

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
int poll_connections(struct pollfd ufds[], const int total_connections, const int timeout) {
    int poll_status;
    
    poll_status = poll(ufds, total_connections, timeout < -1 ? 0 : timeout);    // -1 timeout is infinite

    if(poll_status == -1) {
        log_warn(errno, LOGSTR_POLLCONNECTIONS);
        return -1;
    }
    else if(poll_status == 0) {
        log_info(INFO_BOTH, "No response within 1000 ms\n");
    }

    log_info(INFO_BOTH, "Polled %d connections, %d have data\n", total_connections, poll_status);

    return poll_status;
}