#include <stdio.h>
#include <stdbool.h>
#include "header.h"

extern bool checkPacketValidity();

int main() {
    char exPacket1[] = "\x05\x64\x00\x00\x00\x00\x00\x00\x00\x00" ;
        // can also be written as "\x05\x64\x00..."
        //{0x05, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
    // char exPacket2[11] = 0xasl;
    
    bool vaild = checkPacketValidity(exPacket1);
}