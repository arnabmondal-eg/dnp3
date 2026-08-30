#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#include "packet.h"
#include "log.h"

#ifndef freqPackets_h
#define freqPackets_h

dnp3p_st dnp3Lib_setNavigation(dnp3p_st, int, int, int);

header_st dnp3Lib_mkResetLink(int, int);
header_st dnp3Lib_mkAck(int, int, int);
header_st dnp3Lib_mkNack(int, int, int);

int request_data(uint8_t *, int, int, int, int, int, int);
int write_data(uint8_t *, uint8_t *, uint64_t[], int);

#endif