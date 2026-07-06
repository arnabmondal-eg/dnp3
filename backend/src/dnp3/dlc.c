#include "dlc.h"
#include "binaryHelper.h"

/**
 * @brief Extracts bit-by-bit information from DLC byte. 
 * 
 * @param hexInput Hexs to parse from
 */
dlc_st mkDLC(uint8_t hexInput[]) {
    dlc_st dlc_s;

    memcpy(&dlc_s, &hexInput[DLC_START], 1);    // copy 1 byte over

    return dlc_s;
}

/**
 * @brief Prints Data extracted from DLC bytes of DNP3 Header
 * 
 * @param dlc_s DLC Struct to print
 */
void printDLCData(dlc_st dlc_s) {
    const char *PRIMARY_FNC_CODES[16] = {
        "Reset Link States", 
        "", 
        "Test Link States", "Confirmed User Data", "Unconfirmed User Data",
        "", "", "", "", 
        "Request Link Status",
         "", "", "", "", "", "",
    };

    const char *SECONDARY_FNC_CODES[16] = {
        "Acknowledge", "Negative Acknowledge",
        "", "", "", "", "", "", "", "", "", 
        "Link Status",
        "", "", "", 
        "Not Supported"
    };
    printf("---- DLC ----\n");

    printf("Direction: [%d] %s\n", 
        dlc_s.dirBit, 
        dlc_s.dirBit == 1 ? "From Master" : "From Outstation"
    );

    printf("Primarity: [%d] %s\n",
        dlc_s.prmBit, 
        dlc_s.prmBit == 1 ? "Initiating" : "Responding"
    );

    printf("%s: [%d]\n",
        dlc_s.prmBit == 1 ? "Frame Count" : "Zero-Bit",
        dlc_s.fcbBit
    );

    // bit 4
    if (dlc_s.prmBit) {
        printf("Frame Count Valid: [%d] %s\n",
            dlc_s.fcv_dfcBit,
            dlc_s.fcv_dfcBit == 1 ? "Yes" : "No"
        );
    }
    else {
        printf("Data Flow Control: [%d] %s\n",
            dlc_s.fcv_dfcBit,
            dlc_s.fcv_dfcBit == 1 ? "Receive Buffer Full" : "Recieve Buffer Availible"
        );
    }

    //function codes
    if(dlc_s.prmBit) {
        printf("Primary Function Code: [0x%01X] %s\n", 
            dlc_s.fncCodeBits,
            strcmp(PRIMARY_FNC_CODES[dlc_s.fncCodeBits], "\0") ? PRIMARY_FNC_CODES[dlc_s.fncCodeBits] : "No Matching Function Code\n"
        );
    }
    else {
        printf("Secondary Function Code: [0x%01X] %s\n", 
            dlc_s.fncCodeBits,
            strcmp(SECONDARY_FNC_CODES[dlc_s.fncCodeBits], "\0") ? SECONDARY_FNC_CODES[dlc_s.fncCodeBits] : "No Matching Function Code\n"
        );
    }
}

//TODO: Add bit extraction function