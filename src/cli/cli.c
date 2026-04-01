
//  DataRank
//  Programming Training Exercise, TUT, 2026
//  Update  |   2026-04-01

#define STRING_MAXLEN 128

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "../core/core.h"

bool Cli_Input(char cmd[STRING_MAXLEN], void(*callback)()) {
    
    // Refresh
    system("cls");
    callback();

    // Command
    printf("\n> ");
    fgets(cmd, STRING_MAXLEN, stdin);
    cmd[strcspn(cmd, "\n")] = '\0';
    return strcmp(cmd, "close") != 0;
}

bool Cli_Input_Part(char cmd[STRING_MAXLEN], void(*callback)(char cmd[])) {
    
    // Refresh
    system("cls");
    callback(cmd);

    // Command
    printf("\n> ");
    fgets(cmd, STRING_MAXLEN, stdin);
    cmd[strcspn(cmd, "\n")] = '\0';
    return strcmp(cmd, "close") != 0;
}

void Cli_Start() {

    printf(" /$$$$$$$              /$$               /$$$$$$$                      /$$      \n");
    printf("| $$__  $$            | $$              | $$__  $$                    | $$      \n");
    printf("| $$  \\ $$  /$$$$$$  /$$$$$$    /$$$$$$ | $$  \\ $$  /$$$$$$  /$$$$$$$ | $$   /$$\n");
    printf("| $$  | $$ |____  $$|_  $$_/   |____  $$| $$$$$$$/ |____  $$| $$__  $$| $$  /$$/\n");
    printf("| $$  | $$  /$$$$$$$  | $$      /$$$$$$$| $$__  $$  /$$$$$$$| $$  \\ $$| $$$$$$/ \n");
    printf("| $$  | $$ /$$__  $$  | $$ /$$ /$$__  $$| $$  \\ $$ /$$__  $$| $$  | $$| $$_  $$ \n");
    printf("| $$$$$$$/|  $$$$$$$  |  $$$$/|  $$$$$$$| $$  | $$|  $$$$$$$| $$  | $$| $$ \\  $$\n");
    printf("|_______/  \\_______/   \\___/   \\_______/|__/  |__/ \\_______/|__/  |__/|__/  \\__/\n");
    printf("\n2026 Release 1\n");
    printf("--------------\n");
    
    printf("\n");
    printf("\033[32m"); printf("[选择模块]"); printf("\033[0m\n");
    printf("[0] 所有成绩表\n");
}

void Cli_Help() {

    printf("Main -> Help\n");
    printf("------------\n\n");

    char cmds[][STRING_MAXLEN] = {
        "close\t\t回到上一页或关闭程序",
        "help\t\t查看已定义的指令",
        "mod [number]\t进入编号为 number 的模块",
    };
    int cnt = sizeof(cmds) / STRING_MAXLEN;
    for (int i = 0; i < cnt; i++) {
        if (i % 2 == 0) printf("\033[36m");
        printf(cmds[i]);
        printf("\033[0m\n");
    }
}

void Cli_Main() {
    
    char cmd[STRING_MAXLEN] = "\0";
    while (Cli_Input(cmd, Cli_Start)) {
        
        // help
        if (strcmp(cmd, "help") == 0) {
            while (Cli_Input(cmd, Cli_Help)) { }
        }

        // mod
        else if (strncmp(cmd, "mod", 3) == 0) {
            while (Cli_Input_Part(cmd, Core_Main)) { }
        }

        // 未知指令
        else {
            printf("\033[91m未知指令！输入 help 指令以查询。\033[0m");
            Sleep(1000);
        }
    }
}
