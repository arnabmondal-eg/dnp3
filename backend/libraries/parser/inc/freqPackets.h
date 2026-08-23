#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#include "packet.h"

#ifndef freqPackets_h
#define freqPackets_h

dnp3p_st dnp3Lib_setNavigation(dnp3p_st, int, int, int);

header_st dnp3Lib_mkResetLink(int, int);
header_st dnp3Lib_mkAck(int, int, int);
header_st dnp3Lib_mkNack(int, int, int);

dnp3p_st dnp3Lib_mkAnalogReadRequest(int, int, int, int, int);  // des, src, start, stop, varr
int request_data(uint8_t *, int, int, int, int, int, int);

#endif