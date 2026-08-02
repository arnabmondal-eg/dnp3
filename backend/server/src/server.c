#include "server.h"

void recivePacket(int client_sock, uint8_t buffer[]) {
    header_st *header_sp = {0};
    ssize_t recvSize = 0;
    int packetLength = 0;
    fd_set read_fds;
    int ret = 0;

    FD_ZERO(&read_fds);
    FD_SET(client_sock, &read_fds);

    printf("Waiting for Select...\n");
    ret = select(client_sock+1, &read_fds, NULL, NULL, NULL); //wait till data is sent

    if(ret == 0) {
        printf("Socket Timed Out\n");
        return;
    }
    else if(ret < 0) {
        printf("Select Error: %d\n", ret);
        return;
    }
    else {
        printf("Select Return: %d\n", ret);
        if(FD_ISSET(client_sock, &read_fds)) {
            printf("Socket is Ready for Reading\n");
        }
        else {
            printf("Socket is not Ready for Reading\n");
            return;
        }
    }
    
    // First Read 10 bytes and then read remaining if any left
    recvSize = recv(client_sock, &buffer[0], 10, 0);

    // printf("Recived %d Bytes\n", recvSize);
    // printf("Packet:\n");
    // printRawPacket(buffer);

    packetLength = getPacketSize(buffer);

    if(packetLength == 10) {
        return;
    } 
    else {
        recvSize = recv(client_sock, &buffer[10], packetLength-10, 0);
    }

    // printf("Recived %d Bytes, Wanted %d Bytes\n", recvSize, packetLength-10);
    // printf("Packet:\n");
    // printRawPacket(buffer);

    return;
}

void sendReply(int client_sock, uint8_t response[]) {
    send(client_sock, response, getPacketSize(response), 0);
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
    char* end;
    int port = args >= 2 ? strtol(argv[1], &end, 0) : PORT;

    int server_sock = 0;
    int client_sock = 0;
    struct sockaddr_in server;
    uint8_t buffer[1024] = {0};
    uint8_t responseBuffer[296] = {0};

    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // bind socket to port
    bind(server_sock, (struct sockaddr*)&server, sizeof(server));

    // start listening
    listen(server_sock, 1);
    printf("Server listening on port %d...\n", port);

    client_sock = accept(server_sock, NULL, NULL);  // allocate the cliet port (i think)
    printf("Client Connected\n");
    
    while(1) {
        recivePacket(client_sock, buffer);
        processDLC(buffer, responseBuffer);
        sendReply(client_sock, responseBuffer);
    }


    
    close(server_sock);
    close(client_sock);
}