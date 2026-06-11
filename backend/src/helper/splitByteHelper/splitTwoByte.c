#include "splitTwoByte.h"

/**
 * Splits a 2 byte (16 bit) integer into a single byte (8 bit) interger
 * @param twoByte The integer to be split
 * @param splitIndex The byte to be returned. Zero Indexed
 */
uint8_t splitTwoByte(uint16_t twoByte, int splitIndex) {
    uint8_t returnByte;
    if (splitIndex == 0) {
        returnByte = (twoByte >> 8) & 0xFF;  // shift 1 byte over and remove all but last byte
        // compiler will chop of first byte (all the 0s), and keep the rest
    }
    else {
        returnByte = twoByte & 0xFF;
    }

    return returnByte;
}