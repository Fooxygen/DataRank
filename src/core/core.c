
//  DataRank
//  Programming Training Exercise, TUT, 2026
//  Update  |   2026-04-01

#include <stdio.h>
#include <windows.h>

#include "core.h"

void Core_Tables() {
    printf("Main -> Tables\n");
    printf("--------------\n\n");
    
    if (database.cnt_table) {

    }
    else {
        printf("¡¤------------¡¤\n");
        printf("| ÉÐÎ´´´½¨±í |\n");
        printf("¡¤------------¡¤\n");
    }
}

void Core_Main(char cmd[]) {

    int idx = 0;
    int cnt = sscanf(cmd, "%*s %d", &idx);
    if (cnt == 1) {
        if (idx == 0) Core_Tables();
    }
    else {
        printf("!!!!");
        Sleep(2000);
    }
}
