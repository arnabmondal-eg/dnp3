#include "network.h"

#define DEBUG 1


#define LOGSTR_CONNECT_TO_SERVER "network/connect_to_server"

#define LOGSTR_CONNECT_TO_CLIENT "network/connect_to_client"
#define LOGSTR_SETUP_SERVER "network/setup_server"

/**
 * @brief Connects to iPV4 TCP server through socket
 * 
 * @param server_sp Socket adress structre
 * @param server_socket Socket for server connection
 * @param server_adress iPV4 Adress
 * @param port Port
 * @return int -1 on error, 0 else
 */
int connect_to_server(struct sockaddr_in *server_sp, int *server_socket, const char *server_adress, const int port) {
    int adr_status = 0;
    
    server_sp->sin_family = AF_INET;
    server_sp->sin_port = htons(port);

    // setup server adress
    adr_status = inet_pton(AF_INET, server_adress, &server_sp->sin_addr);
    if(adr_status <=0) {
        if(adr_status == 0) errno = 14;

        log_warn(errno, LOGSTR_CONNECT_TO_SERVER);
        return -1;
    }

    // setup socket
    *server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(DEBUG) log_info(INFO_BOTH, "Connecting to Server...\n");
    if(connect(*server_socket, (struct sockeaddr *)server_sp, sizeof(struct sockaddr_in)) == -1) {
        log_warn(errno, LOGSTR_CONNECT_TO_SERVER);

        close(*server_socket);
        return -1;
    }

    if(DEBUG) log_info(INFO_BOTH, "Connected to Server: %s:%d\n", server_adress, port);

    return 0;
}

/**
 * @brief Set the up server object
 * 
 * @param server_sp Socket adress strucutre
 * @param server_socket Socket to bind
 * @param port Port to open
 * @param max_connections Max connections to server
 * @return int -1 on error, 0 else
 */
int setup_server(struct sockaddr_in *server_sp, int *server_socket, const int port, const int max_connections) {
    // configure server socket
    *server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1) {
        log_warn(errno, LOGSTR_SETUP_SERVER);
        return -1;
    }

    // configure server
    server_sp->sin_family = AF_INET;
    server_sp->sin_addr.s_addr = INADDR_ANY;
    server_sp->sin_port = htons(port);

    // bind socket to port
    if(bind(*server_socket, (struct sockaddr*)server_sp, sizeof(struct sockaddr_in)) == -1) {
        log_warn(errno, LOGSTR_SETUP_SERVER);
        return -1;
    }

    // start listening on port
    if(listen(*server_socket, max_connections > 0 ? max_connections : 1) == -1) {
        log_warn(errno, LOGSTR_SETUP_SERVER);
        return -1;
    };
    if(DEBUG) log_info(INFO_BOTH, "Server Listening on Port: %d\n", port);

    return 0;
}

/**
 * @brief Connects to a client on a specified socket
 * 
 * @param server_socket Server Socket
 * @param client_socket Client Socket
 * @return int -1 on error, 0 else
 */
int connect_to_client(int *server_socket, int *client_socket) {
    if(DEBUG) log_info(INFO_BOTH, "Connecting to Client\n");

    *client_socket = accept(*server_socket, NULL, NULL);
    if(*client_socket == -1) {
        log_warn(errno, LOGSTR_CONNECT_TO_CLIENT);
        return -1;
    }

    if(DEBUG) log_info(INFO_BOTH, "Connected to Client\n");

    return 0;
}