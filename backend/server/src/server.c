#include "server.h"

int main() {
    int server_sock = 0;
    int client_sock = 0;
    struct sockaddr_in server;
    char buffer[1024] = {0};

    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // bind socket to port
    bind(server_sock, (struct sockaddr *)&server, sizeof(server));

    // start listening
    listen(server_sock, 1);
    printf("Server listening on port %d...\n", PORT);

    // for(int i = 0; i < 5; i++) {
    //     replydnp3(&server_sock, &client_sock, &server, buffer);
    // }

    client_sock = accept(server_sock, NULL, NULL);  // allocate the cliet port (i think)
    printf("Client Connected\n");

    recv(client_sock, buffer, sizeof(buffer), 0);    // hardcoded buffer size, fix soon
    printf("Recived:\n%s", buffer);

    printf("Recived 5 Messaged. Closing Connection\n");

    close(server_sock);
    close(client_sock);
}

void replydnp3(int *server_sock, int *client_sock, struct sockaddr_in *server, char buffer[]) {
    *client_sock = accept(*server_sock, NULL, NULL);  // allocate the cliet port (i think)
    printf("Client Connected\n");

    recv(*client_sock, buffer, 1024, 0);    // hardcoded buffer size, fix soon
    printf("Recived:\n%s", buffer);

    return;
}