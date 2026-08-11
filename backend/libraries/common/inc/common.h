#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#include "log.h"
#include "packet.h"
#include "freqPackets.h"
#include "rawPacketHelper.h"

#define COMMON_MAX_CONNECTIONS 128

#ifndef common_h
#define common_h

int init_connection(int, struct pollfd[]);
int send_packet(const int, const uint8_t []);
int recieve_packet(const int, uint8_t []);
int interpret_packet(uint8_t [], uint8_t []);

int poll_connections(struct pollfd*, int, const int);

#endif