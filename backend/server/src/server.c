#include "server.h"

#define MAX_POINTS 128

#define LOGSTR_RECIVEPACKET "serve/recievePacket"
#define LOGSTR_SENDREPLY "serve/sendReply"
#define LOGSTR_PROCESSDLC "serve/processDLC"
#define LOGSTR_MAIN "serve/main"

void close_server(int server_sock, int client_sock) {
    log_program_terminate("Server");
    
    close(server_sock);
    close(client_sock);
}

int main(int args, char** argv) {
    errno = 0;
    
    uint8_t recieve_buffer[296] = {0};
    uint8_t response_buffer[296] = {0};

    int server_sock = 0;
    int client_sock = 0;
    struct sockaddr_in server_address;
    int port;
    
    char* end;

    int num_connections = 0;
    struct pollfd connections[1];
    int poll_status = 0;

    server_def_st server_s = {0};
    uint64_t points[MAX_POINTS];
    int total_points = 0;


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

    // create server_s
    log_info(INFO_BOTH, "Configuring Server\n");

        // get data type
        printf("Data point type: ");
        scanf("%d", &server_s.type);
        if(server_s.type < 0) {
            log_info(INFO_BOTH, "Invalid type: %d\n", server_s.type);
            log_info(INFO_BOTH, "Setting to default: Type 1 Analog\n");
            server_s.type = 1;
        }
        log_info(INFO_BOTH, "Server point type: %d\n", server_s.type);

        // get variation
        printf("Data point variation: ");
        scanf("%d", &server_s.variation);
        if(server_s.variation < 0) {
            log_info(INFO_BOTH, "Invalid Variation: %d\n", server_s.variation);
            log_info(INFO_BOTH, "Setting to defualt: Variation 2\n");
            server_s.variation = 1;
        }
        log_info(INFO_BOTH, "Server point variation: %d\n", server_s.variation);

        // get start
        printf("Point Start: ");
        scanf("%d", &server_s.start);
        if(server_s.start < 0) {
            log_info(INFO_BOTH, "Start too Small: %d\n", server_s.start);
            log_info(INFO_BOTH, "Setting to Default: 0\n");
            server_s.start = 0;
        }
        log_info(INFO_BOTH, "Server point start: %d\n", server_s.start);

        // get end
        printf("Point End: ");
        scanf("%d", &server_s.end);
        if(server_s.end < 0 || server_s.end <= server_s.start) {
            log_info(INFO_BOTH, "End too Small: %d\n", server_s.end);
            log_info(INFO_BOTH, "Setting to Default: %d\n", server_s.start+10);
            server_s.end = server_s.start + 10;
        }
        else if(server_s.end - server_s.start > MAX_POINTS) {
            log_info(INFO_BOTH, "End too Large, max of %d points supported\n", MAX_POINTS);
            log_info(INFO_BOTH, "Setting to Max: %d\n", server_s.start + MAX_POINTS);
            server_s.end = server_s.start + MAX_POINTS;
        }
        log_info(INFO_BOTH, "Server point end: %d\n", server_s.end);

        total_points = server_s.end - server_s.start;


    log_info(INFO_BOTH, "Finished Configuring Server\n");

    if(setup_server(&server_address, &server_sock, port, 1) != 0) {
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

    // sending server data to client
    log_info(INFO_BOTH, "Sending Server Data to Client\n");
    if(send(client_sock, (uint8_t *)&server_s, sizeof(server_def_st), 0) == -1) {
        log_err(errno, "Server");
        close_server(server_sock, client_sock);
    }
    
    while(1) {
        errno = 0;

        // poll connections
        poll_status = poll_connections(connections, num_connections, -1);
        if(poll_status == -1 || poll_status == 0) {
            continue;
        }

        // create data
        refresh_data(server_s.type, server_s.variation, total_points, points);

        if(recieve_packet(connections[0].fd, recieve_buffer) != 0) {
            log_info(INFO_BOTH, "Recieve Failed, closing\n");
            break;
        }

        if(server_interpret(recieve_buffer, response_buffer, total_points, points) != 0) {
            log_info(INFO_BOTH, "Interpret Failed, continuing\n");
            continue;
        };

        if(send_packet(client_sock, response_buffer) != 0) {
            log_info(INFO_BOTH, "Send Failed, closing\n");
            break;
        };
    }

    close_server(server_sock, client_sock);
}