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

    if(server_num > total_servers) {
        errno = 19;
        log_warn(errno, "client_helper/choose_server");
        return -1;
    }

    printf("\n");

    return server_num;
}

int choose_request() {
    int request_num = 0;

    printf("Choose Request Type: \n");
    printf("1. Reset Link\n");
    printf("2. Request Data\n");
    printf("3. Back\n");
    printf(": ");
    scanf("%d", &request_num);
    printf("\n");

    return request_num;
}

/**
 * @brief Interprets Packet and Creates response (or hands off to other function)
 * 
 * @param recieve_buffer Packet to Interpret
 * @param send_buffer Created Packet
 * @return int Returns -1 if Error, 0 Else
 */
int client_interpret(uint8_t recieve_buffer[], uint8_t send_buffer[]) {
    header_st *header_sp = {0};
    int prm = 0;

    header_sp = (header_st *) recieve_buffer;   // map recieve to header struct
    prm = header_sp->dlc_s.prm;
    

    switch (header_sp->dlc_s.function_code) {

        case 0:
            if(prm == 1) {
                log_info(INFO_BOTH, "Recived reset link, not responding\n");
                return -1;
            }
            else if(prm = 0) {
                log_info(INFO_BOTH, "Recived ACK\n");
            }
            break;
            
        case 3:
        case 4:
            if(prm = 1) {
                log_info(INFO_BOTH, "Recieved response with server data\n");
            }

            break;
        
        default:
            errno = 22;
            log_warn(errno, "interpret_packet");

            break;
    }

    return 0;
}