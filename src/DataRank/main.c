
//  DataRank
//  Programming Training Exercise
//  Tianjin University of Technology | 2026
//  Update  |   2026-04-09

#include "public.h"
#include <stdio.h>
#include <windows.h>

#include "cli/cli.h"
#include "core/core.h"
#include "module/module.h"

struct Database database;

int main() {

    // Init
    SetConsoleTitleA("DataRank");
    Core_Open();

    // Cli
    void(*callback)() = Cli_Main;
    char cmd[STRING_MAXLEN] = "\0";
    while (Cli_Input(cmd, callback)) {

        // main
        if (strcmp(cmd, "main") == 0)
            callback = Cli_Main;

        // help
        else if (strcmp(cmd, "help") == 0)
            callback = Cli_Help;

        // mod
        else if (strcmp(cmd, "mod 1") == 0)
            callback = Module_TableDirectory;
        else if (strcmp(cmd, "mod 2") == 0)
            callback = Module_TableCreate;
        else if (strcmp(cmd, "mod 3") == 0)
            callback = Module_TableInsert;
        else if (strcmp(cmd, "mod 4") == 0)
            callback = Module_TableOutput;

        // 未知指令
        else {
            OUTPUT_RED("未知指令！");
            Sleep(1000);
        }
    }

    // Save
    Core_Close();

    return 0;
}
