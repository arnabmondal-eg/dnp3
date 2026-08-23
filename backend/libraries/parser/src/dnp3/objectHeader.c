#include "objectHeader.h"

objectHeader_st mkObjectHeader(uint8_t hexInput[], int *caretPosition) 
{
    const int RANGE_SIZE[12] = {2, 4, 8, 1, 2, 4, 0, 1, 2, 4, 0, 0};  // 10 is not a range code, 11 is undefined
    objectHeader_st object_header_s = {0};
    int shiftAmnt = 0;

    // int temp;    
    // dlc_st dlc_s = mkDLC(hexInput, &temp);

    // int dir = dlc_s.dir != 0 ? 1 : 0;
    int dir = 1;

    // no inn bytes in applheader
    if(dir) shiftAmnt = 2;  // if from primary, shift by 2 as there are no inn bytes

    memcpy(&object_header_s, &hexInput[*caretPosition], 2); // set group and variation
        *caretPosition += 2;
    
    object_header_s.qualPrefix = hexInput[*caretPosition] >> 4;    // capture first 4 bits
    object_header_s.qualRangeCode = hexInput[*caretPosition];      // capture last 4 bits
        *caretPosition += 1;
    //fill range bytes
    // memcpy(&object_header_s.range, &hexInput[RANGE_START-shiftAmnt], RANGE_SIZE[object_header_s.qualRangeCode]);

    // start index
    if(object_header_s.qualRangeCode <= 3) {
        memcpy(&object_header_s.rangeStart, &hexInput[*caretPosition], RANGE_SIZE[object_header_s.qualRangeCode]/2);
            *caretPosition += RANGE_SIZE[object_header_s.qualRangeCode]/2;
        memcpy(&object_header_s.rangeStop, &hexInput[*caretPosition], RANGE_SIZE[object_header_s.qualRangeCode]/2);
            *caretPosition += RANGE_SIZE[object_header_s.qualRangeCode]/2;
        
        object_header_s.numberOfPoints = object_header_s.rangeStop - object_header_s.rangeStart + 1;
        
        // printf("Range Index: %01X\n", object_header_s.qualPrefix);
        // printf("Range Code: %01X\n", object_header_s.qualRangeCode);

        // printf("Range Size: %d\n", RANGE_SIZE[object_header_s.qualRangeCode]);
        // // printf("Range Size: %lu\n", object_header_s.numberOfPoints);
        // printf("Range Start: %02x\n", object_header_s.rangeStart);
        // printf("Range Stop: %02x\n", object_header_s.rangeStop);
    }
    else {
        memcpy(&object_header_s.rangeStart, &hexInput[*caretPosition], RANGE_SIZE[object_header_s.qualRangeCode]);
            *caretPosition += RANGE_SIZE[object_header_s.qualRangeCode];
        
        object_header_s.numberOfPoints = object_header_s.rangeStart;
        
        object_header_s.rangeStart = 0;
        object_header_s.rangeStop = 0;
    }


    // stop index

    
    return object_header_s;
}

void printObjectHeader(objectHeader_st objHeader_s) {
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

    printf("Range Start: %02x\n", objHeader_s.rangeStart);
    printf("Range Stop: %02x\n", objHeader_s.rangeStop);
    printf("Number of Points: %lu\n", objHeader_s.numberOfPoints);
}

