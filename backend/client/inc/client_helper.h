#include <stdio.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "log.h"
#include "packet.h"
#include "freqPackets.h"

#ifndef client_helper_h
#define client_helper_h

void close_client(struct pollfd [], int );\
int client_interpret(uint8_t *, uint8_t *);

int choose_server(int);
int choose_request();

#endif