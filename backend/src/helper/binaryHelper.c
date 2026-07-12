#include "binaryHelper.h"
#include <stdio.h>

uint8_t maskArr[] = {
    BIT_7_MASK, BIT_6_MASK, BIT_5_MASK, BIT_4_MASK,
    BIT_3_MASK, BIT_2_MASK, BIT_1_MASK, BIT_0_MASK
};

/**
 * @brief Converts a passed 1 bit int array into binary array
 * 
 * @param inputHexArray Hex values to be converted
 * @param length Length of inptu
 * @param binary Array to write to
 */
void hexToBinary(uint8_t inputHexArray[], int length, char *binary) {
    int cntr = 0;

    // number of bits
    for(int i = 1; i <= length; i++) {

        // printf("%02X -> ", inputHexArray[i-1]);

        // 8 bits in a byte
        for (int j = 1; j <= 8; j++) {
            binary[cntr] = (inputHexArray[i-1] & maskArr[j-1]) != 0 ? 1 : 0;

            // printf("%d  ", binary[cntr]);

            cntr++;
        }

        // printf("\n");
    }
}


// void byteToBinary(uint8_t inputHex, uint8_t *binary) {
//     int cntr = 0;

//     printf("Value = %02X ->", inputHex);
//     for(int i = 0; i < 8; i++) {
//         binary[cntr] = (inputHex & maskArr[i]) != 0 ? 1 : 0;
//         printf("Vlaue = %d, mask: %02X\n", inputHex, maskArr[7-i]);
//         printf("%d ", binary[cntr]);
//         cntr++;
//     }
//     printf("\n");
// }