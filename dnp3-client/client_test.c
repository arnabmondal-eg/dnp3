#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 12356

int main() {
    int sock;
    struct sockaddr_in server;
    char buffer[1024] = {0};

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    inet_pton(AF_INET, "0", &server.sin_addr);

    // Connect to server
    connect(sock, (struct sockaddr *)&server, sizeof(server));

    printf("Connected to server!\n");

    // Send message
    send(sock, "Hello, Server!", 14, 0);

    // Receive reply
    recv(sock, buffer, sizeof(buffer), 0);

    printf("Server says: %s\n", buffer);

    close(sock);

    return 0;
}