#include "objectHeader.h"

dnp3objh_st mkObjectHeader(uint8_t hexInput[]) {
    dnp3objh_st objectHeader_s;
    
    dnp3h_st mainHeader_s = mkHeader(hexInput); // use just to read dir bit from dlc 
        dnp3hDLC_st dlc_s = mkDLC(mainHeader_s);

    int dir = dlc_s.dirBit != 0 ? 1 : 0;

    if(dir){ memcpy(&objectHeader_s, &hexInput[OBJECT_HEADER_1_START]-2, 3);

    printf("Dir 1");}

    else memcpy(&objectHeader_s, &hexInput[OBJECT_HEADER_1_START], 3);    // no inn bytes in applheader

    return objectHeader_s;
}

void printObjectHeader(dnp3objh_st objHeader_s) {
    const char PREFIX[7][25] = {
        "No Index; Packed", "1-Octect Index", "2-Octect Index", 
        "4-Octect Index", "1-Octect; Object Size", 
        "2-Octect; Object Size", "4-Octect; Object Size",
    };
    
    printf("Group: %d\n", objHeader_s.group);
    if((objHeader_s.group != 30) && (objHeader_s.group != 1)) printf("       ^---- NOTE: This Group is unsuported\n");
    printf("Variation: %d\n", objHeader_s.variation);

    printf("Prefix: [%01X] %s\n", objHeader_s.qualPrefix, PREFIX[objHeader_s.qualPrefix]);
    printf("Range Code: %01X\n", objHeader_s.qualRangeCode);
}

