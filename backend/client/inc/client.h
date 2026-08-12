#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>

#include "rawPacketHelper.h"
#include "freqPackets.h"
#include "packet.h"
#include "log.h"
#include "network.h"
#include "common.h"
#include "client_helper.h"

#ifndef client_h
#define client_h

#define PORT 48188  // https://www.random.org/ (0-65535)

#endif