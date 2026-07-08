#include "data.h"

void printData(uint8_t hexInput[], int *caretPosition) {
    int temp = 0;
    objectHeader_st objHeader_s = mkObjectHeader(hexInput, &temp);

    int startIndex = 0;
    int endIndex = 0;

    int datapoints = 0;
    data0101_st *data0101_sp = 0;
    data0102_st *data0102_sp = 0;

    int i = 0;
    int j = 0;
    
    switch (objHeader_s.group) {
        
        case 1: /* Object Group 1 */
        
            datapoints = objHeader_s.numberOfPoints;

            if (objHeader_s.variation == 1) {
                data0101_sp = (data0101_st *) &hexInput[*caretPosition];
                printf("~~~~ Values of Data ~~~~");
                for (i = 0; i < datapoints / 8; i++) {
                    printf("~~ Group %d ~~", i);
                    printf(
                        "pt1: %d, pt2: %d, pt3: %d, pt4: %d",
                        data0101_sp->data0, data0101_sp->data1, data0101_sp->data2, data0101_sp->data3
                    );
                    printf(
                        "pt5: %d, pt6: %d, pt7: %d, pt8: %d",
                        data0101_sp->data4, data0101_sp->data5, data0101_sp->data6, data0101_sp->data7
                    );
                    data0101_sp = data0101_sp + sizeof(data0101_st);
                } 
            }
            
            else if(objHeader_s.variation == 2) {
                
            }
            break;
        }
}