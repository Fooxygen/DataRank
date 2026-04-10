
//  DataRank
//  Programming Training Exercise
//  Tianjin University of Technology | 2026
//  Update  |   2026-04-09

#include "cli.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

// 启动页
bool Cli_Main() {

    printf("Main\n");
    printf("----\n\n");

    OUTPUT_CYAN(" /$$$$$$$              /$$               /$$$$$$$                      /$$      \n");
    OUTPUT_CYAN("| $$__  $$            | $$              | $$__  $$                    | $$      \n");
    OUTPUT_CYAN("| $$  \\ $$  /$$$$$$  /$$$$$$    /$$$$$$ | $$  \\ $$  /$$$$$$  /$$$$$$$ | $$   /$$\n");
    OUTPUT_CYAN("| $$  | $$ |____  $$|_  $$_/   |____  $$| $$$$$$$/ |____  $$| $$__  $$| $$  /$$/\n");
    OUTPUT_CYAN("| $$  | $$  /$$$$$$$  | $$      /$$$$$$$| $$__  $$  /$$$$$$$| $$  \\ $$| $$$$$$/ \n");
    OUTPUT_CYAN("| $$  | $$ /$$__  $$  | $$ /$$ /$$__  $$| $$  \\ $$ /$$__  $$| $$  | $$| $$_  $$ \n");
    OUTPUT_CYAN("| $$$$$$$/|  $$$$$$$  |  $$$$/|  $$$$$$$| $$  | $$|  $$$$$$$| $$  | $$| $$ \\  $$\n");
    OUTPUT_CYAN("|_______/  \\_______/   \\___/   \\_______/|__/  |__/ \\_______/|__/  |__/|__/  \\__/\n");
    OUTPUT_GREEN("\n2026 Release\n");
    printf("\n在"); OUTPUT_YELLOW(" > ") printf("后可输入指令");
    printf("\n使用"); OUTPUT_YELLOW(" 'help' ") printf("可查看所有支持的指令");
    printf("\n--------------------------------\n");

    printf("\n");
    OUTPUT_CYAN("[ 可用的模块 ]\n");
    printf("[ 1 ] 查看成绩表目录\n");
    printf("[ 2 ] 创建成绩表\n");
    printf("[ 3 ] 录入成绩表\n");
    printf("[ 4 ] 查询成绩表\n");

    return true;
}

// 帮助页
bool Cli_Help() {

    printf("Main -> Help\n");
    printf("------------\n\n");

    char cmds[][STRING_MAXLEN] = {
        "close\t\t| 结束程序",
        "main\t\t| 启动页",
        "mod [number]\t| 进入编号为 number 的模块",
    };
    int cnt = sizeof(cmds) / STRING_MAXLEN;
    for (int i = 0; i < cnt; i++) {
        if (i % 2 == 0) { OUTPUT_CYAN(cmds[i]); }
        else printf(cmds[i]);
        printf("\n");
    }

    return true;
}

// 清除全屏
void Cli_Clear() {
    system("cls");
}

// 获取输入的 [Y/N]，非 y 或 Y 则认为是 n
bool Cli_SelectReturn() {
    char c = '\0'; scanf("%c", &c); getchar();
    if (c == 'y' || c == 'Y') return true;
    return false;
}

// 获取命令输入
bool Cli_Input(char cmd[STRING_MAXLEN], void(*callback)()) {

    // Refresh
    system("cls");
    callback();

    // Command
    OUTPUT_YELLOW("\n> ");
    fgets(cmd, STRING_MAXLEN, stdin);
    cmd[strcspn(cmd, "\n")] = '\0';
    return strcmp(cmd, "close") != 0;
}

// 获取数字显示的长度
int  Cli_IntLength(int x) {

    return snprintf(NULL, 0, "%d", x);
}
int  Cli_FloatLength(float x, int prec) {

    return snprintf(NULL, 0, "%.*f", prec, x);
}
