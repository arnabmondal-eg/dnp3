#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "log.h"
#include "packet.h"
#include "freqPackets.h"

#ifndef server_helper_h
#define server_helper_h

void refresh_data(int, int, int, uint64_t *);
int server_interpret(uint8_t *, uint8_t *, int, uint64_t *);

#endif