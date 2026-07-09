#include "data.h"

void printData(uint8_t hexInput[], int *caretPosition, objectHeader_st *objectHeader_sp) {
    int temp = 0;

    int startIndex = 0;
    int endIndex = 0;

    int datapoints = 0;

    data0101_st *data0101_sp = 0;
    data0102_st *data0102_sp = 0;

    data3001_st *data3001_sp = 0;
    data3002_st *data3002_sp = 0;
    
    int i = 0;
    int j = 0;
    
    printf("---- Values of Data ----\n");
    datapoints = objectHeader_sp -> numberOfPoints;

    switch (objectHeader_sp -> group) {
        
        case 1: /* Object Group 1 */
        
            if (objectHeader_sp -> variation == 1) {
                data0101_sp = (data0101_st *) &hexInput[*caretPosition];

                for (i = 0; i < datapoints / 8; i++) {
                    printf("-- Group %d --\n", i);
                    printf(
                        "pt1: %d, pt2: %d, pt3: %d, pt4: %d\n",
                        data0101_sp->data0, data0101_sp->data1, data0101_sp->data2, data0101_sp->data3
                    );
                    printf(
                        "pt5: %d, pt6: %d, pt7: %d, pt8: %d\n",
                        data0101_sp->data4, data0101_sp->data5, data0101_sp->data6, data0101_sp->data7
                    );
                    data0101_sp = data0101_sp + sizeof(data0101_st);
                    *caretPosition += 1;
                } 
            }
            
            else if(objectHeader_sp -> variation == 2) {
                data0102_sp = (data0102_st *) &hexInput[*caretPosition];

                for(i = 0; i < datapoints; i++) {
                    printf("Point #%d: %d\n", i, data0102_sp -> state);
                    printf(
                        "\tFlags (CF LF RF CL R ON): %d %d %d %d %d %d\n", 
                        data0102_sp->chatterFiler, data0102_sp->localForced, data0102_sp->remoteForced,
                        data0102_sp->commLost, data0102_sp->restart, data0102_sp->online
                    );
                    data0102_sp = data0102_sp + sizeof(data0102_st);
                    *caretPosition += 1;
                }
            }
            
            break;
        
        case 30:
        
        if (objectHeader_sp -> variation == 1) {
            data3001_sp = (data3001_st *) &hexInput[*caretPosition];
            
            for(i =0; i < datapoints; i++) {
                printf("Point #%d: %d\n", i, data3001_sp -> value);
                printf(
                    "\tFlags (RE OR LF RF CL R ON): %d %d %d %d %d %d %d\n", 
                    data3001_sp->referenceErr, data3001_sp->overRange, data3001_sp->localForced, 
                    data3001_sp->remoteForced, data3001_sp->commLost, data3001_sp->restart, data3001_sp->online
                );
                data3001_sp = data3001_sp + sizeof(data3001_st);
                *caretPosition += 1;
            }
        }

        else if(objectHeader_sp -> variation == 2) {
            data3002_sp = (data3002_st *) &hexInput[*caretPosition];

            for(i =0; i < datapoints; i++) {
                printf("Point #%d: %d\n", i, data3002_sp -> value);
                printf(
                    "\tFlags (RE OR LF RF CL R ON): %d %d %d %d %d %d %d\n", 
                    data3002_sp->referenceErr, data3002_sp->overRange, data3002_sp->localForced, 
                    data3002_sp->remoteForced, data3002_sp->commLost, data3002_sp->restart, data3002_sp->online
                );
                data3002_sp = data3002_sp + sizeof(data3001_st);
                *caretPosition += 1;
            }
        }
    }
        
}