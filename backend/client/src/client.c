#include "client.h"

int main(int args, char **argv) {
    errno = 0;
    
    uint8_t send_buffer[296];
    uint8_t recieve_buffer[296];

    header_st header_s = {0};

    char* end;
    int port = 0;
    char address[20] = {0};

    int menu_input = 0;
    int menu_server = 0;

    int total_servers = 0;
    int failed_connections = 0;
    struct sockaddr_in server[COMMON_MAX_CONNECTIONS];
    struct pollfd server_poll[COMMON_MAX_CONNECTIONS];
    server_def_st server_info[COMMON_MAX_CONNECTIONS];
    
    // setup logging
    if(log_init("log/client_log.txt") == -1) return -1;

    // log start
    log_program_start("Client");
    
    // setup connection amount
    if(args >= 2) {
        total_servers = strtol(argv[1], &end, 0);
        if(total_servers <= 0) total_servers = 1;
        log_info(INFO_BOTH, "Using custom number of  connections: %d\n", total_servers);
    }
    else total_servers = 1;

    // setup all connections
    log_info(INFO_BOTH, "Setting up %d connection(s)\n", total_servers);
    for(int i=0; i<total_servers; i++) {

        // get port
        printf("Port for Server %d: ", i+1);
        scanf("%d", &port);

        // get adress
        printf("Server Adress (0 for Local): ");
        scanf("%s", address);
        if(strcmp(address, "0") == 0) {
            strcpy(address, "127.0.0.1");
        }

        log_info(INFO_BOTH, "Attempting to connect to Server %d at %s:%d\n", i, address, port);
        if(connect_to_server(&(server[i]), &(server_poll[i].fd), address, port) != 0) {
            log_warn(errno, "client/main");
            close(server_poll[i].fd);

            failed_connections++;
        }

        server_poll[i].events = POLLIN;
    }

    // make sure at least 1 server was connected to
    if(failed_connections == total_servers) {
        log_info(INFO_BOTH, "Could not Connect to any Servers!\n");
        close_client(server_poll, total_servers);
        return 0;
    }

    // collect server data from each server
    log_info(INFO_BOTH, "Attempting to Receive Server Information\n");
    for(int i = 0; i < total_servers; i++) {
        recv(server_poll[i].fd, (uint8_t *) &server_info[i], sizeof(server_def_st), 0);
        server_info[i].num = i;

        log_info(INFO_BOTH, "Server %d: \n", i);
        log_info(INFO_BOTH, "\tType: %d\n", server_info[i].type);
        log_info(INFO_BOTH, "\tStart: %d\n", server_info[i].start);
        log_info(INFO_BOTH, "\tEnd: %d\n", server_info[i].end);
    }

    menu_server = choose_server(total_servers);

    while(menu_server != 0) {
        // poll_connections(server_poll, total_servers, 500);

        if(menu_input == 3) {
            menu_server = choose_server(total_servers);

            if(menu_server == 0) break;
        }
        menu_input = choose_request();

        switch (menu_input)
        {
        case 0:
            break;
        case 1:
            {
                header_s = dnp3Lib_mkResetLink(server_info[menu_server-1].num, 0);
                log_info(INFO_BOTH, "Sending Packet to Server %d", menu_server);
                if(send_packet(server_poll[menu_server-1].fd, (uint8_t *)&header_s) != 0) {
                    log_info(INFO_BOTH, "Failed to Send Packet\n");
                    continue;
                }

                poll_connections(server_poll, total_servers, 100);
                if(server_poll[menu_server-1].revents == POLLIN) {
                    if(recieve_packet(server_poll[menu_server-1].fd, recieve_buffer) != 0) {
                        log_info(INFO_BOTH, "Failed to Recieve Packet\n");
                        continue;
                    }

                    log_info(INFO_BOTH, "Recived Packet\n");

                    printf("\n Raw Packet:\n");
                    printRawPacket(recieve_buffer);
                }
                else {
                    errno = 61;
                    log_warn(errno, "client/main");
                }
                    
                break;
            }
        
        case 2:
            {
                request_data(
                    send_buffer, 
                    server_info[menu_server-1].type,
                    server_info[menu_server-1].variation,
                    server_info[menu_server-1].start,
                    server_info[menu_server-1].end,
                    0,
                    server_info[menu_server].num
                );
                log_info(INFO_BOTH, "Sending request to server %d for data\n", menu_server);
                if(send_packet(server_poll[menu_server-1].fd, send_buffer) != 0) {
                    log_info(INFO_BOTH, "Failed to send packet\n");
                    continue;
                }

                if(poll_connections(&server_poll[menu_server-1], 1, 1000) > 0) {
                    if(server_poll[menu_server-1].revents & POLLIN) {
                        if(recieve_packet(server_poll[menu_server-1].fd, recieve_buffer) != 0) {
                            log_info(INFO_BOTH, "Failed to Recieve Packet\n");
                            continue;
                        }

                        log_info(INFO_BOTH, "Recived Packet\n");
                        printRawPacket(recieve_buffer);
                    }
                }
                else {
                    errno = 61;
                    log_warn(errno, "client/main");
                }

                break;
            }
        default:
            break;
        }
    }

    close_client(server_poll, total_servers);

    return 0;
}