#include "client.h"

void sendPacket(int sock, uint8_t request[]) {
    log_info(INFO_FILE, "Sending Request...\n");

    if(send(sock, request, getPacketSize(request), 0) == -1) {
        log_err(errno, "client/sendPacket");
        return;
    }

    log_info(INFO_FILE, "Successfuly Sent %d Bytes\n", getPacketSize(request));

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

    log_info(INFO_FILE, "Waiting for Data...\n");
    ret = select(server_sock+1, &read_fds, NULL, NULL, NULL); //wait till data is sent

    if(ret == 0) {
        errno = 110;
        log_err(errno, "client/recievePacket");

        return;
    }
    else if(ret < 0) {
        log_err(errno, "client/recievePacket");

        return;
    }
    else {
        if(FD_ISSET(server_sock, &read_fds)) {
            log_info(INFO_FILE, "Socket is Ready for Reading\n");
        }
        else {
            log_info(INFO_FILE, "Socket is not Ready for Reading\n");

            return;
        }
    }
    
    // First Read 10 bytes and then read remaining if any left
    recvSize = recv(server_sock, &buffer[0], 10, 0);
    if(recvSize == -1) {
        log_err(errno, "client/recievePacket");

    }
    else if(recvSize == 0) {
        errno  = 61;
        log_warn(errno, "client/recievePacket");
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

int main(int args, char **argv) {
    errno = 0;
    
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
    
    FILE *temp_fptr;

    int sock;
    int length;
    struct sockaddr_in server;
    char* end;
    int port = 0;
    char* address = {0};

    header_st header_s = {0};

    int input = 0;
    int src = 0;
    int des = 1;
    
    // setup logging
    if(log_init("log/client_log.txt") == -1) {
        return -1;
    };

    log_program_start("Client");

    if (args >= 2) {
        port = strtol(argv[1], &end, 0);
        log_info(INFO_BOTH, "Using Custom Port: %d\n", port);
    }
    else {
        port = PORT;
    }

    // check and set custom address
    if (args >= 3) {
        address = argv[2];
        log_info(INFO_BOTH, "Using Custom Server Adress: %s\n", address);
    }
    else {
        address = "127.0.0.1";
    }

    // setup server
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // setup server address
    int addrStatus =  inet_pton(AF_INET, address, &server.sin_addr);
    if (addrStatus <= 0) {
        if(addrStatus == 0) errno = 14;
        
        log_err(errno, "client/main");

        return -1;
    }
    

    // setup socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // attempt to connect to server
    log_info(INFO_BOTH, "Attempting to Connect to Server...\n");

    if(connect(sock, (struct sockaddr *)&server, sizeof(server)) == -1) {      // deref should give correct size
        
        log_err(errno, "client [main]");

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
                log_info(INFO_FILE, "Client Sent: \n");

                // change stream
                temp_fptr = stdout;
                stdout = log_get_file();
                printRawPacket((uint8_t *) &header_s);
                stdout = temp_fptr;

                recivePacket(sock, requestBuffer);
                log_info(INFO_FILE, "Client Recieved: \n");

                temp_fptr = stdout;
                stdout = log_get_file();
                printRawPacket(requestBuffer);
                stdout = temp_fptr;
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

    log_program_terminate("Client");

    return 0;
}
