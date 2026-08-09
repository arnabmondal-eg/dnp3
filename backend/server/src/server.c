#include "server.h"

#define LOGSTR_RECIVEPACKET "serve/recievePacket"
#define LOGSTR_SENDREPLY "serve/sendReply"
#define LOGSTR_PROCESSDLC "serve/processDLC"
#define LOGSTR_MAIN "serve/main"

void recivePacket(int server_sock, uint8_t buffer[]) {
    header_st *header_sp = {0};
    ssize_t recvSize = 0;
    int packetLength = 0;
    fd_set read_fds;
    int ret = 0;

    FD_ZERO(&read_fds);
    FD_SET(server_sock, &read_fds);

    log_info(INFO_CONSOLE, "Waiting for Data...\n");
    ret = select(server_sock+1, &read_fds, NULL, NULL, NULL); //wait till data is sent

    if(ret == 0) {
        errno = 110;
        log_err(errno, LOGSTR_RECIVEPACKET);
        return;
    }
    else if(ret < 0) {
        log_err(errno, LOGSTR_RECIVEPACKET);
        return;
    }
    else {
        // fprintf(fptr, "Select Return: %d\n", ret);
        if(FD_ISSET(server_sock, &read_fds)) {
            log_info(INFO_CONSOLE, "Socket is Ready for Reading\n");
        }
        else {
            log_warn(errno, LOGSTR_RECIVEPACKET);
            return;
        }
    }
    
    // First Read 10 bytes and then read remaining if any left
    recvSize = recv(server_sock, &buffer[0], 10, 0);
    if(recvSize == -1) {
        log_err(errno, LOGSTR_RECIVEPACKET);
    }
    else if(recvSize == 0) {
        errno = 61;
        log_warn(errno, LOGSTR_RECIVEPACKET);
    }

    packetLength = getPacketSize(buffer);

    if(packetLength == 10) {
        return;
    } 
    else {
        recvSize = recv(server_sock, &buffer[10], packetLength-10, 0);
    }

    return;
}

void sendReply(int client_sock, uint8_t response[]) {
    int sendStatus;

    sendStatus = send(client_sock, response, getPacketSize(response), 0);
    if(sendStatus == -1) {
        log_err(errno, LOGSTR_SENDREPLY);
    }

    return;
}

void processDLC(uint8_t packet[], uint8_t response[]) {
    header_st *header_sp = {0};

    header_sp = (header_st *) packet;

    switch(header_sp->dlc) {

        case 0xC0:  // reset link
            header_st ack_s = {0};

            ack_s = dnp3Lib_mkAck(0, header_sp->des, header_sp->src);
            memcpy(response, &ack_s, 10);

        break;
    }
}

int main(int args, char** argv) {
    errno = 0;
    
    uint8_t buffer[1024] = {0};
    uint8_t responseBuffer[296] = {0};

    int server_sock = 0;
    int client_sock = 0;
    struct sockaddr_in server;
    
    int port;

    char* end;
    FILE *temp_fptr;
    time_t currentTime;

    // setup logging
    if(log_init("log/server_log.txt") == -1) {
        return -1;
    };

    // append time and date  to file
    log_program_start("Server");

    // check and set custom port
    if(args >= 2) {
        port = strtol(argv[1], &end, 0);
        log_info(INFO_BOTH, "Using Custom Port: %d\n", port);
    }
    else {
        port = PORT;
    }

    if(setup_server(&server, &server_sock, port, 1) != 0) {
        log_err(errno, LOGSTR_MAIN);
        return -1;
    }

    if(connect_to_client(server_sock, client_sock) != 0) {
        log_err(errno, LOGSTR_MAIN);
        return -1;
    }
    
    for(int i = 0; i<2; i++) {
        recivePacket(client_sock, buffer);
        processDLC(buffer, responseBuffer);
        sendReply(client_sock, responseBuffer);
    }

    log_program_terminate("Server");
    
    close(server_sock);
    close(client_sock);
}