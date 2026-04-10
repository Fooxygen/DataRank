
//  DataRank
//  Programming Training Exercise
//  Tianjin University of Technology | 2026
//  Update  |   2026-04-10

#include "module.h"
#include <stdio.h>
#include <windows.h>

#include "../cli/cli.h"
#include "../core/core.h"

// 表目录输出
void Module_TableDirectory() {

    printf("Main -> TableDirectory\n");
    printf("----------------------\n\n");

    int cnt = database.table_cnt;
    if (cnt) {
        for (int i = 0; i < cnt; i++) {
            Core_TableMetaOutput(&database.tables[i]);
            printf("\n");
        }
    }
    else {
        OUTPUT_YELLOW("| 目录下无任何表 |\n");
    }
}

// 表输出
void Module_TableOutputTitle() {

    printf("Main -> TableOutput\n");
    printf("-------------------\n\n");
}
void Module_TableOutput() {

    // data
    int   idx = -1;
    int   order_mode = 0;
    int   predicate_mode = 0;
    float predicate_x = 0, predicate_y = 0;

    // 设定表编号
    {
        while (true) {
            Cli_Clear();
            Module_TableOutputTitle();

            OUTPUT_CYAN("[ 进度 ]\n");
            printf(
                OUTPUT_YELLOW_STR"◆ 设定表编号"OUTPUT_WHITE_STR
                " -> "
                "[◆ 设定排序方式]"
                " -> "
                "[◆ 设定过滤方式]"
                " -> "
                "◆ 完成\n"
            );

            printf("\n请输入要查询的表编号：");
            scanf("%d", &idx);
            getchar();

            if (Core_TableExist(idx)) {
                OUTPUT_GREEN("表编号有效！");
                Sleep(1000);
                break;
            }
            else {
                OUTPUT_RED("表编号不存在或超过输入范围");
                Sleep(1000);
            }
        }
    }

    // 可选设定排序方式
    {
        while (true) {
            Cli_Clear();
            Module_TableOutputTitle();

            OUTPUT_CYAN("[ 进度 ]\n");
            printf(
                OUTPUT_GREEN_STR"◆ 设定表编号"OUTPUT_WHITE_STR
                " -> "
                OUTPUT_YELLOW_STR"[◆ 设定排序方式]"OUTPUT_WHITE_STR
                " -> "
                "[◆ 设定过滤方式]"
                " -> "
                "◆ 完成\n"
            );

            OUTPUT_YELLOW("\n现在是可选步骤！");
            OUTPUT_YELLOW("\n若要跳过该步骤，请输入 'end'");
            printf("\n你可以从如下 4 种方式选择排序方式\n");
            printf("\n[ 1 ] 按选手编号升序");
            printf("\n[ 2 ] 按选手编号降序");
            printf("\n[ 3 ] 按成绩升序，同成绩则按选手编号升序");
            printf("\n[ 4 ] 按成绩降序，同成绩则按选手编号升序");
            printf("\n\n请选择排序方式：");

            // input
            char  input[STRING_MAXLEN] = { '\0' };
            fgets(input, STRING_MAXLEN, stdin);
            input[strcspn(input, "\n")] = '\0';

            if (strcmp(input, "end") == 0) break;
            else {
                sscanf(input, "%d", &order_mode);
                if (order_mode >= 1 && order_mode <= 4) break;
                else {
                    OUTPUT_RED("排序方式不存在或超过输入范围！");
                    Sleep(1000);
                }
            }
        }
    }

    // 可选设定过滤区间
    {
        while (true) {
            Cli_Clear();
            Module_TableOutputTitle();

            OUTPUT_CYAN("[ 进度 ]\n");
            printf(
                OUTPUT_GREEN_STR"◆ 设定表编号"OUTPUT_WHITE_STR
                " -> "
                OUTPUT_GREEN_STR"[◆ 设定排序方式]"OUTPUT_WHITE_STR
                " -> "
                OUTPUT_YELLOW_STR"[◆ 设定过滤区间]"OUTPUT_WHITE_STR
                " -> "
                "◆ 完成\n"
            );

            OUTPUT_YELLOW("\n现在是可选步骤！");
            OUTPUT_YELLOW("\n若要跳过该步骤，请输入 'end'");
            printf("\n你可以从如下 2 种方式选择过滤方式，并设定区间");
            OUTPUT_YELLOW("\n例如输入：1 2 3，即只输出选手编号在 [2, 3] 的数据\n");
            printf("\n[ 1 ] 按选手编号过滤");
            printf("\n[ 2 ] 按成绩过滤");
            printf("\n\n请选择过滤方式并设定区间：");

            // input
            char  input[STRING_MAXLEN] = { '\0' };
            fgets(input, STRING_MAXLEN, stdin);
            input[strcspn(input, "\n")] = '\0';

            if (strcmp(input, "end") == 0) break;
            else {
                sscanf(input, "%d %f %f", &predicate_mode, &predicate_x, &predicate_y);
                if (predicate_mode < 1 || predicate_mode > 2) {
                    OUTPUT_RED("过滤方式不存在或超过输入范围！");
                    Sleep(1000);
                    continue;
                }
                if (predicate_y < predicate_x) {
                    OUTPUT_RED("过滤区间不合法！");
                    Sleep(1000);
                    continue;
                }
                break;
            }
        }
    }

    // 补全文件数据并输出
    {
        Cli_Clear();
        Module_TableOutputTitle();

        OUTPUT_CYAN("[ 进度 ]\n");
        printf(
            OUTPUT_GREEN_STR"◆ 设定表编号"OUTPUT_WHITE_STR
            " -> "
            OUTPUT_GREEN_STR"[◆ 设定排序方式]"OUTPUT_WHITE_STR
            " -> "
            OUTPUT_GREEN_STR"[◆ 设定过滤方式]"OUTPUT_WHITE_STR
            " -> "
            OUTPUT_GREEN_STR"◆ 完成\n"OUTPUT_WHITE_STR
        );

        struct Table* table = Core_TableGet(idx);
        Core_TableFileLoad(table);
        Core_TableMetaOutput(table);
        Core_TableOutput(table, order_mode, predicate_mode, predicate_x, predicate_y);
    }
}

// 表创建
void Module_TableCreateTitle() {

    printf("Main -> TableCreate\n");
    printf("-------------------\n\n");
}
void Module_TableCreate() {

    // data
    int  idx = -1, judge_cnt = 0;
    struct Table table = {
        .header = { idx = -1, .player_cnt = 0, .judge_cnt = 0 },
        .isFileLoad = true, .isFileEqualMemory = false,        // 所有数据仅在内存中，故认为完整读取，尚未刷写
        .scores = NULL
    };

    // 设定表编号
    {
        while (true) {
            Cli_Clear();
            Module_TableCreateTitle();

            Core_TableMetaOutput(&table);

            OUTPUT_CYAN("[ 进度 ]\n");
            printf(
                OUTPUT_YELLOW_STR"◆ 设定表编号"OUTPUT_WHITE_STR
                " -> "
                "◆ 设定裁判人数"
                " -> "
                "◆ 完成\n"
            );

            printf("\n请输入要创建的表编号：");
            scanf("%d", &idx);
            getchar();

            if (!Core_TableExist(idx)) {
                OUTPUT_GREEN("表编号有效！");
                Sleep(1000);
                table.header.idx = idx;
                break;
            }
            else {
                OUTPUT_YELLOW("表编号已存在！是否覆盖？[Y/N]：");
                if (Cli_SelectReturn()) {
                    table.header.idx = idx;
                    break;
                }
            }
        }
    }

    // 设定裁判人数
    {
        while (true) {
            Cli_Clear();
            Module_TableCreateTitle();

            Core_TableMetaOutput(&table);

            OUTPUT_CYAN("[ 进度 ]\n");
            printf(
                OUTPUT_GREEN_STR"◆ 设定表编号"OUTPUT_WHITE_STR
                " -> "
                OUTPUT_YELLOW_STR"◆ 设定裁判人数"OUTPUT_WHITE_STR
                " -> "
                "◆ 完成\n"
            );

            printf("\n请输入裁判人数：");
            scanf("%d", &judge_cnt);
            getchar();

            if (!Core_TableJudgeValid(judge_cnt)) {
                OUTPUT_RED("裁判人数必须大于 2！");
                Sleep(1000);
            }
            else {
                OUTPUT_GREEN("裁判人数有效！");
                Sleep(1000);
                table.header.judge_cnt = judge_cnt;
                break;
            }
        }
    }

    // 完成
    {
        Cli_Clear();
        Module_TableCreateTitle();

        Core_TableMetaOutput(&table);

        OUTPUT_CYAN("[ 进度 ]\n");
        printf(
            OUTPUT_GREEN_STR"◆ 设定表编号"OUTPUT_WHITE_STR
            " -> "
            OUTPUT_GREEN_STR"◆ 设定裁判人数"OUTPUT_WHITE_STR
            " -> "
            OUTPUT_GREEN_STR"◆ 完成\n"OUTPUT_WHITE_STR
        );

        // expand
        Core_DatabaseExpand(1);

        // write
        database.tables[database.table_cnt - 1] = table;
    }
}

// 表插入
void Module_TableInsertTitle() {

    printf("Main -> TableInsert\n");
    printf("-------------------\n\n");
}
void Module_TableInsert() {

    // data
    int table_idx = -1;
    struct Table* table = NULL;

    // 设定表编号
    {
        while (true) {
            Cli_Clear();
            Module_TableInsertTitle();

            OUTPUT_CYAN("[ 进度 ]\n");
            printf(
                OUTPUT_YELLOW_STR"◆ 设定表编号"OUTPUT_WHITE_STR
                " -> "
                "◆ 插入数据"
                " -> "
                "◆ 完成\n"
            );

            printf("请输入要插入的表编号：");
            scanf("%d", &table_idx);
            getchar();

            if (Core_TableExist(table_idx)) {
                OUTPUT_GREEN("表编号有效！");
                Sleep(1000);
                table = Core_TableGet(table_idx);
                break;
            }
            else {
                OUTPUT_RED("表编号不存在或超过输入范围");
                Sleep(1000);
            }
        }
    }

    // 补全文件数据
    {
        Core_TableFileLoad(table);
    }

    // 插入数据
    {
        while (true) {
            Cli_Clear();
            Module_TableInsertTitle();

            OUTPUT_CYAN("[ 进度 ]\n");
            printf(
                OUTPUT_GREEN_STR"◆ 设定表编号"OUTPUT_WHITE_STR
                " -> "
                OUTPUT_YELLOW_STR"◆ 插入数据"OUTPUT_WHITE_STR
                " -> "
                "◆ 完成\n"
            );

            Core_TableMetaOutput(table);
            Core_TableOutput(table, PLAYERORDER_EMPTY, PLAYERPREDICATE_EMPTY, 0, 0);

            printf("\n现在可逐选手输入成绩");
            printf("\n若要结束输入，请输入 'end'");
            printf("\n请依次输入选手的编号，每位裁判给出的成绩：");

            // input
            char  input[STRING_MAXLEN] = { '\0' };
            char* pinput = input;
            fgets(input, STRING_MAXLEN, stdin);
            input[strcspn(input, "\n")] = '\0';
            
            // 接收到数字组合
            int   player_idx = 0;
            float scores[LIST_MAXSIZE] = { 0.0f };
            if (sscanf(input, "%d", &player_idx) == 1) {

                int offset = 0;     // 读取偏移
                int write_idx = 0;  // 写入下标

                // idx_player
                {
                    sscanf(input, "%d%n", &player_idx, &offset);
                    pinput += offset;

                    write_idx = Core_TablePlayerExist(table, player_idx);
                    if (write_idx == -2) {
                        OUTPUT_RED("选手编号超过输入范围！");
                        Sleep(1000);
                        continue;
                    }
                    else if (write_idx != -1) {
                        OUTPUT_YELLOW("选手编号已存在！是否覆盖？[Y/N]：");
                        if (!Cli_SelectReturn()) continue;
                    }
                }

                // score + score...
                {
                    for (int i = 0; i < table->header.judge_cnt; i++) {
                        sscanf(pinput, "%f%n", &scores[i], &offset);
                        pinput += offset;
                        if (scores[i] < 0) {
                            OUTPUT_RED("有一项或多项输入的成绩超出范围！");
                            Sleep(1000);
                            continue;
                        }
                    }
                }

                // write
                int player_cnt_new = table->header.player_cnt + (write_idx == -1 ? 1 : 0);
                if (write_idx == -1) write_idx = player_cnt_new - 1;

                // 扩容
                Core_TableExpand(table, player_cnt_new, table->header.judge_cnt);

                // player idx 写入
                table->players[write_idx] = player_idx;

                // scores 写入
                memcpy(&table->scores[write_idx * table->header.judge_cnt], scores,
                    sizeof(float) * table->header.judge_cnt);
            }

            // 接收到结束命令
            else if (strcmp(input, "end") == 0) {
                break;
            }
        }
    }

    // 完成
    {
        Cli_Clear();
        Module_TableInsertTitle();

        OUTPUT_CYAN("[ 进度 ]\n");
        printf(
            OUTPUT_GREEN_STR"◆ 设定表编号"OUTPUT_WHITE_STR
            " -> "
            OUTPUT_GREEN_STR"◆ 插入数据"OUTPUT_WHITE_STR
            " -> "
            OUTPUT_GREEN_STR"◆ 完成\n"OUTPUT_WHITE_STR
        );

        Core_TableMetaOutput(table);
        Core_TableOutput(table, PLAYERORDER_EMPTY, PLAYERPREDICATE_EMPTY, 0, 0);

        table->isFileEqualMemory = false;
    }
}
