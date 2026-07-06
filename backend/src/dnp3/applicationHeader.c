#include "applicationHeader.h"

extern void byteToBinary(uint8_t input, char *binary);

const char APP_FUNCTION_CODES[37][30]= {
    "CONFIRM", "READ", "WRITE", "SELECT", "OPERATE", 
    "DIRECT OPERATE", "DIRECT OPERATE_NO_ACK", "IMMEDIATE FREEZE", "IMMEDIATE FREEZE_NO_ACK", "FREEZE CLEAR", 
    "FREEZE CLEAR_NO ACK", "FREEZE AT TIME", "FREEZE_AT_TIME_NO_ACK", "COLD RESTART", "WARM RESTART", 
    "INITIALIZE DATA", "INITIALIZE APPLICATION", "START APPLICATION", "STOP APPLICATION", "SAVE CONFIGURATION", 
    "ENABLE UNSOLICITED", "DISABLE UNSOLICITED", "ASSIGN CLASS", "DELAY MEASURE", "RECORD CURRENT TIME", 
    "OPEN FILE", "CLOSE FILE", "DELETE FILE", "GET FILE INFO", "AUTHENTICATE FILE", 
    "ABORT FILE", "ACTIVE CONFIGURATON","AUTHENTICATION REQUEST", "AUTHENTICATION ERROR","RESPONSE", 
    "UNSOLICITED RESPONSE", "AUTHENTICATE RESPONSE"
};

const char INN1_RESPONSES[8][15] = {
    "ALL STATIONS", "CLASS 1", "CLASS 2", "CLASS 3", 
    "NEED TIME", "LOCAL", "DEVICE TROUBLE", "RESTART"
};
const char INN2_RESPONSES[8][17] = {
    "BAD FUNCTION", "OBJECT UNKNOWN", "OUT OF RANGE", "BUFFER OVERFLOW", 
    "ALREADY EXECU", "BAD CONFIG", "RESERVED", "RESERVED"
};

applicationHeader_st mkApplicationHeader(uint8_t hexInput[]) {
    applicationHeader_st applHeader_s;
        
    dlc_st dlc_s = mkDLC(hexInput);

    int dir = dlc_s.dirBit != 0 ? 1 : 0;
    
    if(dir == 1) {
        memcpy(&applHeader_s, &hexInput[APPLHDR_START], 2);
        applHeader_s.innActive = 0;
    }
    else {
        memcpy(&applHeader_s, &hexInput[APPLHDR_START], 4);    // if rtu to main, get inn bytes
        applHeader_s.innActive = 1;
    }

    return applHeader_s;
}

void printApplicationHeader(applicationHeader_st applHeader_s) {
    printf("---- Application Header ----\n");

    // Application Control Byte

    printf("First: %02X\n", applHeader_s.acFirst);

    // 1. First Bit
    // TODO: Figure out what is the difference between appl header first and final vs transport header first and final
    if(applHeader_s.acFirst != 0) printf("First Fragment\n");
    else printf("Not First Fragment\n");

    // 2. Final Bit
    if(applHeader_s.acFinal != 0) printf("Final Fragment\n");
    else printf("Not Final Fragment\n");

    // 3. Application Confirm Bit
    if(applHeader_s.acApplConfirm != 0) printf("Application Confirmed\n");
    else printf("Application Not Confrimed\n");

    // 4. Unsolicited Bit
    if(applHeader_s.acUnsolicited != 0) printf("Unsolicited\n");
    else printf("Not Unsolicited\n");

    // 5. Fragment Sequence
    printf("Fragment Sequence: %04X [%d]\n", applHeader_s.acFragmentSequence, applHeader_s.acFragmentSequence);


    // Application Function Code
    printf("Application Function Code: ");
    if(applHeader_s.applicationFunctionCode <= 33) {
        printf("%s\n", APP_FUNCTION_CODES[applHeader_s.applicationFunctionCode]);
    }
    else if(applHeader_s.applicationFunctionCode <= 131 && applHeader_s.applicationFunctionCode >= 129) {
        switch(applHeader_s.applicationFunctionCode) {
            case 0X81:
                printf("%s\n", APP_FUNCTION_CODES[34]);
                break;
            case 0x82:
                printf("%s\n", APP_FUNCTION_CODES[35]);
                break;
            case 0x83:
                printf("%s\n", APP_FUNCTION_CODES[36]);
                break;
            default:
                printf("NO MATCHING CODE\n");
        }
    }

    if(applHeader_s.innActive) {
        printf("Internal Indications:\n");
        uint8_t inn1Binary[8] = {0};
        uint8_t inn2Binary[8] = {0};

        uint8_t temp1 [] = {applHeader_s.inn1};
        uint8_t temp2 [] = {applHeader_s.inn2};


        hexToBinary(temp1, sizeof(temp1), inn1Binary);
        hexToBinary(temp2, sizeof(temp2), inn2Binary);

        printf("-- INN 1 --\n");
        for(int i = 0; i < 8; i++) {
            if(inn1Binary[i] == 1) printf("%s\n", INN1_RESPONSES[i]);
        }

        printf("-- INN 2 --\n");
        for(int i = 0; i < 8; i++) {
            if(inn2Binary[i] == 1) printf("%s\n", INN2_RESPONSES[i]);
        }
    }
}