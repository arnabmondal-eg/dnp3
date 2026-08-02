#include "client.h"

void sendPacket(int sock, uint8_t request[]) {

    printf("Connected to Server\n");

    printf("Sending Request...\n");
    send(sock, request, getPacketSize(request), 0);

    printf("Successfuly Sent %d Bytes\n", getPacketSize(request));

    // close(sock);
    return;
}

void recivePacket(int server_sock, uint8_t buffer[]) {
    header_st *header_sp = {0};
    ssize_t recvSize = 0;
    int packetLength = 0;
    fd_set read_fds;
    int ret = 0;

    FD_ZERO(&read_fds);
    FD_SET(server_sock, &read_fds);

    printf("Waiting for Select...\n");
    ret = select(server_sock+1, &read_fds, NULL, NULL, NULL); //wait till data is sent

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
        if(FD_ISSET(server_sock, &read_fds)) {
            printf("Socket is Ready for Reading\n");
        }
        else {
            printf("Socket is not Ready for Reading\n");
            return;
        }
    }
    
    // First Read 10 bytes and then read remaining if any left
    recvSize = recv(server_sock, &buffer[0], 10, 0);

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

int main(int args, char **argv) {
    uint8_t requestBuffer[296];
    uint8_t replyBuffer[296];
    uint8_t request1[] = {
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
    
    FILE *fptr;
    int sock;
    int length;
    struct sockaddr_in server;
    char* end;
    int port = args >= 2 ? strtol(argv[1], &end, 0) : PORT;
    char* address = args >= 3 ? argv[2] : "127.0.0.1";

    header_st header_s = {0};

    int input = 0;
    int src = 0;
    int des = 1;

    // setup server
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // setup server address
    if (inet_pton(AF_INET, address, &server.sin_addr) <= 0) {
        printf("Invalid Address\n");
        return -1;
    }

    errno = 0;

    // setup logging
    fptr = fopen("log/client_log.txt", "a");

    // setup socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // attempt to connect to server
    fprintf(fptr, "Attempting to Connect to Server (Port: %d)...\n", port);

    if(connect(sock, (struct sockaddr *)&server, sizeof(server)) == -1) {      // deref should give correct size
        
        fprintf(fptr,"Connection Failed!\n");
        errno = 111;
        perror("client [sendPacket]");

        close(sock);
        return 0;
    }

    while(input != 4) {
        printf("Choose Request Type: \n");
        printf("1. Reset Link\n");
        printf("2. Analog Read\n");
        printf("3. Digital Read\n");
        printf("4. End\n");
        printf(": ");
        scanf("%d", &input);

        switch(input) {

            case 1:
                header_s = dnp3Lib_mkResetLink(des, src);
                sendPacket(sock, (uint8_t *) &header_s);
                printf("Client Sent: \n");
                printRawPacket((uint8_t *) &header_s);

                recivePacket(sock, requestBuffer);
                printf("Client Recived: \n");
                printRawPacket(requestBuffer);
            break;

            case 2:
                
            break;
            
            case 3:

            break;

            case 4:
            default:
            break;
        }
    }

    close(sock);

    // sendPacket(port, &server, request1);
    // printf("\n");
    // sendPacket(port, &server, reply);

    return 0;
}
