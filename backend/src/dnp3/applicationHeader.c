#include "applicationHeader.h"

dnp3aph_st mkApplicationHeader(uint8_t hexInput[]) {
    dnp3aph_st applHeader_s;
        
    dnp3h_st mainHeader_s = mkHeader(hexInput); // use just to read dir bit from dlc 
        dnp3hDLC_st dlc_s = mkDLC(mainHeader_s);

    int dir = dlc_s.dirBit != 0 ? 1 : 0;
    
    if (dir == 1) {
        applHeader_s.innActive = 0; // if from primary, no indications

        applHeader_s.inn1 = 0;      // 0 out to prevent strange errors
        applHeader_s.inn2 = 0;      // if accesed
    }
    else {
        applHeader_s.innActive = 1;
        applHeader_s.inn1 = hexInput[APPLHDR_INN_1];
        applHeader_s.inn2 = hexInput[APPLHDR_INN_2];
    }

    applHeader_s.acFirst = hexInput[APPLHDR_APPL_CNTRL_POSE] & BIT_7_MASK;
    applHeader_s.acFinal = hexInput[APPLHDR_APPL_CNTRL_POSE] & BIT_6_MASK;
    applHeader_s.acApplConfirm = hexInput[APPLHDR_APPL_CNTRL_POSE] & BIT_5_MASK;
    applHeader_s.acUnsolicited = hexInput[APPLHDR_APPL_CNTRL_POSE] & BIT_4_MASK;

    applHeader_s.acFragmentSequence = hexInput[APPLHDR_APPL_CNTRL_POSE] & 0x0F;     // 0000 1111 (mask out last 4 bits)

    applHeader_s.applicationFunctionCode = hexInput[APPLHDR_APPL_FUNC_CODE_POSE];   // 37 possible >:(

    applHeader_s.inn1 = hexInput[APPLHDR_INN_1];
    applHeader_s.inn2 = hexInput[APPLHDR_INN_2];
}

void printApplicationHeader(dnp3aph_st applHeader_s) {
    printf("\n---- Application Header ----\n");

    // Application Control Byte

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
    
}