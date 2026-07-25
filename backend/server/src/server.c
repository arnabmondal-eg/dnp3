#include "server.h"

void replydnp3(int *server_sock, int *client_sock, struct sockaddr_in *server, uint8_t buffer[]) {
    *client_sock = accept(*server_sock, NULL, NULL);  // allocate the cliet port (i think)
    printf("Client Connected\n");

    ssize_t recvSize = recv(*client_sock, buffer, 102, 0);    // hardcoded buffer size, fix soon
    printf("Recived %d Bytes\n", recvSize);

    printf("Packet:\n");
    printRawPacket(buffer);
    
    dnp3p_st packet = mkPacketAuto(buffer);
    // printPacket(packet);

    return;
}

int main(int args, char** argv) {
    char* end;
    int port = args >= 2 ? strtol(argv[1], &end, 0) : PORT;

    int server_sock = 0;
    int client_sock = 0;
    struct sockaddr_in server;
    uint8_t buffer[1024] = {0};

    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // bind socket to port
    bind(server_sock, (struct sockaddr*)&server, sizeof(server));

    // start listening
    listen(server_sock, 1);
    printf("Server listening on port %d...\n", port);

    replydnp3(&server_sock, &client_sock, &server, buffer);
    replydnp3(&server_sock, &client_sock, &server, buffer);

    close(server_sock);
    close(client_sock);
}