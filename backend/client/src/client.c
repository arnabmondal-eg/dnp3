#include "client.h"

int main(int args, char **argv) {
    errno = 0;
    
    uint8_t send_buffer[296];
    uint8_t recieve_buffer[296];
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

        printf("\n");

        log_info(INFO_BOTH, "Attempting to connect to Server %d at %s:%d\n", i, address, port);
        if(connect_to_server(&(server[i]), &(server_poll[i].fd), address, port) != 0) {
            log_warn(errno, "client/main");
            close(server_poll[i].fd);

            failed_connections++;
        }
    }

    // make sure at least 1 server was connected to
    if(failed_connections == total_servers) {
        log_info(INFO_BOTH, "Could not Connect to any Servers!\n");
        close_client(server_poll, total_servers);
    }

    menu_server = choose_server(total_servers);

    while(menu_server != 0) {
        poll_connections(server_poll, total_servers, 500);

        if(menu_input == 4) {
            menu_server = choose_server(total_servers);

            if(menu_server == 0) break;
        }
        menu_input = choose_request();

        switch (menu_input)
        {
        case 0:
            break;
        case 1:
            header_s = dnp3Lib_mkResetLink(1, 0);
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
                printRawPacket(recieve_buffer);
            }
            else {
                errno = 61;
                log_warn(errno, "client/main");
            }
                
            break;
        
        default:
            break;
        }
    }

    for(int i = 0; i < total_servers; i++) {
        close(server_poll[i].fd);
    }

    log_program_terminate("Client");

    return 0;
}