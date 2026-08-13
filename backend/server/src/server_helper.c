#include "server_helper.h"

void refresh_data(int type, int total_points, int points[]) {
    int min = 0;
    int max = 0;

    if(type == 1) {
        max = 1;
    }
    else if(type == 30) {
        max = 100;
    }
    else return;
    
    srand(time(NULL));

    for(int i = 0; i < total_points; i++) {
        points[i] = (rand() % (max - min + 1)) + min;
    }
}