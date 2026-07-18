#ifndef client_h
#define client_h

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>

#define PORT 48188  // https://www.random.org/ (0-65535)

void replydnp3(int*, int*, struct sockaddr_in *, char []);

#endif