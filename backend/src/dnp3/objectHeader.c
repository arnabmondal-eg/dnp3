#include "objectHeader.h"

dnp3objh_st mkObjectHeader(uint8_t hexInput[]) {
    const int RANGE_SIZE[12] = {2, 4, 8, 2, 4, 8, 0, 1, 2, 4, 0, 0};  // 10 is not a range code, 11 is undefined
    dnp3objh_st objectHeader_s = {0};
    int shiftAmnt = 0;
    
    dnp3hDLC_st dlc_s = mkDLC(hexInput);

    int dir = dlc_s.dirBit != 0 ? 1 : 0;

    // no inn bytes in applheader
    if(dir) shiftAmnt = 2;  // if from primary, shift by 2 as there are no inn bytes

    memcpy(&objectHeader_s, &hexInput[OBJECT_HEADER_1_START-shiftAmnt], 2); // set group and variation
    
    objectHeader_s.qualPrefix = hexInput[QUALIFER_START-shiftAmnt] >> 4;    // capture first 4 bits
    objectHeader_s.qualRangeCode = hexInput[QUALIFER_START-shiftAmnt];      // capture last 4 bits
    
    //fill range bytes
    memcpy(&objectHeader_s.range, &hexInput[RANGE_START-shiftAmnt], RANGE_SIZE[objectHeader_s.qualRangeCode]);

    return objectHeader_s;
}

void printObjectHeader(dnp3objh_st objHeader_s) {
    static const char PREFIX[7][22] = {
        "No Index; Packed", "1-Octect Index", "2-Octect Index", 
        "4-Octect Index", "1-Octect; Object Size", 
        "2-Octect; Object Size", "4-Octect; Object Size",
    };  // 140 Bytes, smaller than pointer arr and faster access
    // pointer arr would be like 158 bytes

    static const char RANGE_CODE[12][37] = {
        "1-Octect Start & Stop Indices",
        "2-Octect Start & Stop Indices",
        "4-Octect Start & Stop Indices",
        "1-Octect Absolute Address Identifers",
        "2-Octect Absolute Address Identifers",
        "4-Octect Absolute Address Identifers",
        "All Points/Objects", "1-Octect Quantity",
        "2-Octect Quantity", "4-Octect Quantity",
        "ERROR: Invalid Range Code",     // 10 is unsuported
        "Varible Value",    // but 11 is (maniac created this)
    };  // pointer arr would be slightly smaller in this case (392 bytes vs 407)
    
    printf("---- Object Header ----\n");

    printf("Group: [%d]\n", objHeader_s.group);   //TODO: Implement name of group (ie. Binary/Digital, Continous/Analog, ect.)
    if((objHeader_s.group != 30) && (objHeader_s.group != 1)) printf("       ^---- NOTE: This Group is unsuported\n");
    printf("Variation: %d\n", objHeader_s.variation);

    printf("Prefix: [%01X] %s\n", objHeader_s.qualPrefix, PREFIX[objHeader_s.qualPrefix]);
    printf("Range Code: [%01X] %s\n", objHeader_s.qualRangeCode, RANGE_CODE[objHeader_s.qualRangeCode]);

    printf("Range Size: %lu\n", sizeof(objHeader_s.range));
    printf("Range: %0llX\n", objHeader_s.range);
}

