#include "dlc.h"
#include "binaryHelper.h"

/**
 * @brief Extracts bit-by-bit information from DLC byte. 
 * 
 * @param hexInput Hexs to parse from
 */
dlc_st mkDLC(uint8_t hexInput[], int *caretPosition) {
    dlc_st dlc_s;
    memcpy(&dlc_s, &hexInput[*caretPosition-6], 1);    // copy 1 byte over

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
        dlc_s.dir, 
        dlc_s.dir == 1 ? "From Master" : "From Outstation"
    );

    printf("Primarity: [%d] %s\n",
        dlc_s.prm, 
        dlc_s.prm == 1 ? "Initiating" : "Responding"
    );

    printf("%s: [%d]\n",
        dlc_s.prm == 1 ? "Frame Count" : "Zero-Bit",
        dlc_s.fcb
    );

    // bit 4
    if (dlc_s.prm) {
        printf("Frame Count Valid: [%d] %s\n",
            dlc_s.fcv_dfc,
            dlc_s.fcv_dfc == 1 ? "Yes" : "No"
        );
    }
    else {
        printf("Data Flow Control: [%d] %s\n",
            dlc_s.fcv_dfc,
            dlc_s.fcv_dfc == 1 ? "Receive Buffer Full" : "Recieve Buffer Availible"
        );
    }

    //function codes
    if(dlc_s.prm) {
        printf("Primary Function Code: [0x%01X] %s\n", 
            dlc_s.function_code,
            strcmp(PRIMARY_FNC_CODES[dlc_s.function_code], "\0") ? PRIMARY_FNC_CODES[dlc_s.function_code] : "No Matching Function Code"
        );
    }
    else {
        printf("Secondary Function Code: [0x%01X] %s\n", 
            dlc_s.function_code,
            strcmp(SECONDARY_FNC_CODES[dlc_s.function_code], "\0") ? SECONDARY_FNC_CODES[dlc_s.function_code] : "No Matching Function Code"
        );
    }
}

//TODO: Add bit extraction function