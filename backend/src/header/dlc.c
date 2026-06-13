#include "dlc.h"
#include "binaryHelper.h"

/**
 * @brief Extracts bit-by-bit information from DLC byte. 
 * 
 * @param header 
 */
dnp3hDLC_st mkDLC(dnp3h_st header) {
    dnp3hDLC_st dlc_s;

    dlc_s.dirBit = header.dlc & BIT_7_MASK;  // mask out bit 7

    dlc_s.prmBit = header.dlc & BIT_6_MASK;  // mask out bit 6

    dlc_s.fcv_dfcBit = header.dlc & BIT_4_MASK;    // mask bit 4

    if(dlc_s.fcv_dfcBit != 0) {
        dlc_s.fcbBit = header.dlc & BIT_5_MASK;
    }
    else {
        dlc_s.fcbBit = 0;   // if response alwasy 0
    }

    dlc_s.fncCodeBits = header.dlc & DLC_FUNCTION_CODE_MASK;

    return dlc_s;
}

/**
 * @brief Prints Data extracted from DLC bytes of DNP3 Header
 * 
 * @param dlc_s DLC Struct to print
 */
void printDLCData(dnp3hDLC_st dlc_s) {
    // 1. Determine direction
    if (dlc_s.dirBit != 0) {      // bit on
        printf("From Primary\n");
    }
    else {      // bit off
        printf("Secondary\n");
    }

    // 2. Determine primarity (initiation vs responding)
    if (dlc_s.prmBit != 0) {      // bit on
        printf("Primary to Secondary\n");
        dlc_s.response = 0;
    }
    else {      // bit off
        printf("Secondary to Primary\n");
        dlc_s.response = 1;
    }

    // 3. Examine FCV bit
    if(dlc_s.fcv_dfcBit != 0) {
        // 3a. Check FCB Bit
        printf("Examine FCB Bit\n");
        printf("FCB: %d\n", dlc_s.fcbBit != 0 ? 1 : 0);
    }
    else {
        printf("Ignore FCB Bit\n");
    }

    // 4. Check Function Code
    if (dlc_s.response == 0) {
        printf("Primary Function Code: ");
        switch(dlc_s.fncCodeBits) {
            case DLC_RESET_LINK_STATES:
                printf("Rest Link States\n");
                break;
            case DLC_TEST_LINK_STATES:
                printf("Test Link States\n");
                break;
            case DLC_CONFIRMED_USER_DATA:
                printf("Confirmed User Data\n");
                break;
            case DLC_UNCONFIRMED_USER_DATA:
                printf("Unconfirmed User Data\n");
                break;
            case DLC_REQUEST_LINK_STATUS:
                printf("Request Link Status\n");
                break;
            default:
                printf("No Matching Code!\n");     // error: no matching code
        }
    }
    else {
        printf("Secondary Function Code: ");
        switch(dlc_s.fncCodeBits) {
            case DLC_ACK:
                printf("Acknowledge\n");
                break;
            case DLC_NACK:
                printf("Not Acknowledge\n");
                break;
            case DLC_LINK_STATUS:
                printf("Link Status\n");
                break;
            case DLC_NOT_SUPPORTED:
                printf("Not Supported\n");
                break;
            default:
                printf("No Matching Code!\n");     // error: no matching code
        }
    }
}

//TODO: Add bit extraction function