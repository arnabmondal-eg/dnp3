#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "log.h"

#ifndef network_h
#define network_h

int connect_to_server(struct sockaddr_in*, int*, const char*, const int);

int setup_server(struct sockaddr_in*, int*, const int, const int);
int connect_to_client(int*, int*);

#endif