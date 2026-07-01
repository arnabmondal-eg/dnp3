#include "data.h"

void* mkData(uint8_t hexInput[], void *dataAdress) {
    objectHeader_st objHeader_s = mkObjectHeader(hexInput);
    int startIndex = 0;
    int endIndex = 0;
    int indexArr[2] = {startIndex, endIndex};

    // TODO: Update when new types are added
    if (objHeader_s.group != 1) {
        printf("Unsupported Group\n");
        return 0;
    }
    if (objHeader_s.variation != 1 && objHeader_s.variation != 2) {
        printf("Unsupported Varriation\n");
        return 0;
    }

    if (objHeader_s.group == 1) {
        if (objHeader_s.variation == 1) {
            // int startIndex = objHeader_s.range;
        }
    }


}