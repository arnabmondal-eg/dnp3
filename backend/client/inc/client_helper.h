#include <stdio.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "log.h"

#ifndef client_helper_h
#define client_helper_h

void close_client(struct pollfd [], int );

int choose_server(int);
int choose_request();

#endif