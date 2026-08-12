#include "client_helper.h"

void close_client(struct pollfd server_poll[], int total_servers) {
    log_program_terminate("Client");
    
    for(int i = 0; i < total_servers; i++) {
        close(server_poll[i].fd);
    }

}

int choose_server(int total_servers) {
    int server_num = 0;
    
    printf("Server for Request, or 0 to End: ");
    scanf("%d", &server_num);
    printf("\n");

    return server_num;
}

int choose_request() {
    int request_num = 0;

    printf("Choose Request Type: \n");
    printf("1. Reset Link\n");
    printf("2. Analog Read\n");
    printf("3. Digital Read\n");
    printf("4. Back\n");
    printf(": ");
    scanf("%d", &request_num);
    printf("\n");

    return request_num;
}

