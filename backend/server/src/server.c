#include "server.h"

static FILE *fptr;

void recivePacket(int server_sock, uint8_t buffer[]) {
    header_st *header_sp = {0};
    ssize_t recvSize = 0;
    int packetLength = 0;
    fd_set read_fds;
    int ret = 0;

    FD_ZERO(&read_fds);
    FD_SET(server_sock, &read_fds);

    fprintf(fptr, "Waiting for Data...\n");
    ret = select(server_sock+1, &read_fds, NULL, NULL, NULL); //wait till data is sent

    if(ret == 0) {
        errno = 110;
        perror("server [recivePacket]");
        fprintf(fptr, "server [recivePacket]: %s", strerror(errno));
        return;
    }
    else if(ret < 0) {
        perror("server [recievePacket]");
        fprintf(fptr, "server [recievePacket]: %s", strerror(errno));
        return;
    }
    else {
        // fprintf(fptr, "Select Return: %d\n", ret);
        if(FD_ISSET(server_sock, &read_fds)) {
            fprintf(fptr, "Socket is Ready for Reading\n");
        }
        else {
            fprintf(fptr, "Socket is not Ready for Reading\n");
            return;
        }
    }
    
    // First Read 10 bytes and then read remaining if any left
    recvSize = recv(server_sock, &buffer[0], 10, 0);
    if(recvSize == -1) {
        perror("server [recivePacket]");
        fprintf(fptr, "server [recievePacket]: %s", strerror(errno));
    }
    else if(recvSize == 0) {
        fprintf(fptr, "server [recievePacket]: 0 Bytes Recived\n");   // could be shutdown of socket or no data
    }

    // printf("Recived %d Bytes\n", recvSize);
    // printf("Packet:\n");
    // printRawPacket(buffer);

    packetLength = getPacketSize(buffer);

    if(packetLength == 10) {
        return;
    } 
    else {
        recvSize = recv(server_sock, &buffer[10], packetLength-10, 0);
    }

    // printf("Recived %d Bytes, Wanted %d Bytes\n", recvSize, packetLength-10);
    // printf("Packet:\n");
    // printRawPacket(buffer);

    return;
}

void sendReply(int client_sock, uint8_t response[]) {
    int sendStatus;

    sendStatus = send(client_sock, response, getPacketSize(response), 0);
    if(sendStatus == -1) {
        perror("server [sendReply]");
        fprintf(fptr, "server [sendReply]: %s\n", strerror(errno));
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
    fptr = fopen("log/server_log.txt", "a");
    if(fptr == NULL) {
        perror("server [main]");
        return -1;
    }

    // append time and date  to file
    time(&currentTime);
    fprintf(fptr, "\nStarted Server %s\n", ctime(&currentTime));

    // check and set custom port
    if(args >= 2) {
        port = strtol(argv[1], &end, 0);
        fprintf(fptr, "Using Custom Port: %d\n", port);
    }
    else {
        port = PORT;
    }

   // setup server socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock == -1) {
        perror("server [main]");
        fprintf(fptr, "server [main]: %s\n", strerror(errno));
    }

    // configure server
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // bind socket to port
    if(bind(server_sock, (struct sockaddr*)&server, sizeof(server)) == -1) {
        perror("server [main]");
        fprintf(fptr, "server [main]: %s\n", strerror(errno));
    }

    // start listening
    listen(server_sock, 1);
    fprintf(fptr, "Server listening on port %d...\n", port);

    client_sock = accept(server_sock, NULL, NULL);  // allocate the cliet port (i think)
    fprintf(fptr, "Client Connected\n");
    
    for(int i = 0; i<2; i++) {
        recivePacket(client_sock, buffer);
        processDLC(buffer, responseBuffer);
        sendReply(client_sock, responseBuffer);
    }

    fclose(fptr);
    
    close(server_sock);
    close(client_sock);
}