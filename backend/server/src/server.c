#include "server.h"

#define LOGSTR_RECIVEPACKET "serve/recievePacket"
#define LOGSTR_SENDREPLY "serve/sendReply"
#define LOGSTR_PROCESSDLC "serve/processDLC"
#define LOGSTR_MAIN "serve/main"

int main(int args, char** argv) {
    errno = 0;
    
    uint8_t recieve_buffer[296] = {0};
    uint8_t response_buffer[296] = {0};

    int server_sock = 0;
    int client_sock = 0;
    struct sockaddr_in server;
    int port;
    
    char* end;

    int num_connections = 0;
    struct pollfd connections[COMMON_MAX_CONNECTIONS];
    int poll_status = 0;

    // setup logging
    if(log_init("log/server_log.txt") == -1) return -1;

    // append time and date  to file
    log_program_start("Server");

    // check and set custom port
    if(args >= 2) {
        port = strtol(argv[1], &end, 0);
        log_info(INFO_BOTH, "Using Custom Port: %d\n", port);
    }
    else port = PORT;

    if(setup_server(&server, &server_sock, port, 1) != 0) {
        log_err(errno, LOGSTR_MAIN);
        return -1;
    }

    if(connect_to_client(&server_sock, &client_sock) != 0) {
        log_err(errno, LOGSTR_MAIN);
        return -1;
    }

    connections[0].fd = client_sock;
    num_connections = 1;

    init_connection(num_connections, connections);
    
    while(1) {
        errno = 0;

        poll_status = poll_connections(connections, num_connections, -1);
        if(poll_status == -1 || poll_status == 0) {
            continue;
        }

        if(recieve_packet(connections[0].fd, recieve_buffer) != 0) {
            log_info(INFO_BOTH, "Recieve Failed, closing\n");
            break;
        }

        if(interpret_packet(recieve_buffer, response_buffer) != 0) {
            log_info(INFO_BOTH, "Interpret Failed, continuing\n");
            continue;
        };

        if(send_packet(client_sock, response_buffer) != 0) {
            log_info(INFO_BOTH, "Send Failed, closing\n");
            break;
        };
    }

    log_program_terminate("Server");
    
    close(server_sock);
    close(client_sock);
}