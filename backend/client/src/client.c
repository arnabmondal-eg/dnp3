#include "client.h"

int main(int args, char **argv) {
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
    struct sockaddr_in server;

    mkdnp3Request(PORT, &server, reply, sizeof(reply));
}

void mkdnp3Request(int port, struct sockaddr_in *server, uint8_t request[], int length) {
    FILE *fptr;
    int sock;

    // setup logging
    fptr = fopen("log/clog.txt", "a");

    // setup socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // setup server
    server->sin_family = AF_INET;
    server->sin_port = htons(port);

    inet_pton(AF_INET, "0", server);    // dont need mem adress as server is pntr type

    printf("Attempting to Connect to Server (Port: %d)...\n", port);
    connect(sock, (struct sockaddr*)server, sizeof(*server));
    // fprintf(fptr,"Attempting to Connect to Server (Port: %d)...\n", port);
    // if(connect(sock, (struct sockaddr*)server, sizeof(*server))) {      // deref should give correct size
    //     printf("Connection Failed!\n");

    //     close(sock);
    //     return;
    // }

    printf("Connected to Server\n");

    printf("Sending Request...\n");
    for(int i = 0; i < length; i++){
        send(sock, (void*) request[i], 2, 0);
    }

    printf("Successfuly Sent %d Bytes\n", length);

    close(sock);
    return;
}