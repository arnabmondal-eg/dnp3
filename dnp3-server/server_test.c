#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 12356

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    char buffer[1024] = {0};

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to port
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // Start listening
    listen(server_fd, 1);

    printf("Server listening on port %d...\n", PORT);

    // Wait for a client
    client_fd = accept(server_fd, NULL, NULL);

    printf("Client connected!\n");

    // Receive message
    recv(client_fd, buffer, sizeof(buffer), 0);

    printf("Received: %s\n", buffer);

    // Send response
    send(client_fd, "Hello, Client!", 14, 0);

    close(client_fd);
    close(server_fd);

    return 0;
}