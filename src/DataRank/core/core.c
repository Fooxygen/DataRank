
//  DataRank
//  Programming Training Exercise
//  Tianjin University of Technology, 2026
//  Update  |   2026-04-09

#include "core.h"
#include <stdio.h>
#include <windows.h>

#include "../cli/cli.h"

int Kv_Id_Score_AscId(const void* a, const void* b) {
    struct Kv_Player_Score* x = (struct Kv_Player_Score*)a;
    struct Kv_Player_Score* y = (struct Kv_Player_Score*)b;
    return x->player - y->player;
}
int Kv_Id_Score_DescId(const void* a, const void* b) {
    struct Kv_Player_Score* x = (struct Kv_Player_Score*)a;
    struct Kv_Player_Score* y = (struct Kv_Player_Score*)b;
    return y->player - x->player;
}
int Kv_Id_Score_AscScore(const void* a, const void* b) {
    struct Kv_Player_Score* x = (struct Kv_Player_Score*)a;
    struct Kv_Player_Score* y = (struct Kv_Player_Score*)b;

    float score = x->score - y->score;
    if (score < 0) return -1;
    else if (score > 0) return 1;

    return x->player - y->player;                               // Score 相同时，按 Id 升序
}
int Kv_Id_Score_DescScore(const void* a, const void* b) {
    struct Kv_Player_Score* x = (struct Kv_Player_Score*)a;
    struct Kv_Player_Score* y = (struct Kv_Player_Score*)b;

    float score = y->score - x->score;
    if (score < 0) return -1;
    else if (score > 0) return 1;

    return x->player - y->player;                               // Score 相同时，按 Id 升序
}

// 数据库扩容
// 添加 cnt 张表
void Core_DatabaseExpand(int cnt) {

    database.table_cnt += cnt;
    struct Table* tables = realloc(database.tables, sizeof(struct Table) * database.table_cnt);
    if (tables) database.tables = tables;
    else exit(1);
}

// 表扩容
// 将表大小更改为 players * judges
void Core_TableExpand(struct Table* table, int player_cnt, int judge_cnt) {

    table->header.player_cnt = player_cnt;
    table->header.judge_cnt = judge_cnt;

    int* players = realloc(table->players, sizeof(int) * player_cnt);
    if (players) table->players = players;
    else exit(1);

    float* scores = realloc(table->scores, sizeof(float) * player_cnt * judge_cnt);
    if (scores) table->scores = scores;
    else exit(1);
}

// 从文件中填充表的完整数据
void Core_TableFileLoad(struct Table* table) {

    if (!table->isFileLoad) {

        char path[STRING_MAXLEN] = { '\0' };
        snprintf(path, STRING_MAXLEN, "%s/%d%s", FOLDER_TABLES, table->header.idx, FILE_TABLE_SUFFIX);
        FILE* file = fopen(path, "rb");
        if (file) {

            // allocate
            Core_TableExpand(table, table->header.player_cnt, table->header.judge_cnt);

            // jump header
            fseek(file, sizeof(struct TableFileHeader), 0);

            // players
            fread(table->players, sizeof(int), table->header.player_cnt, file);

            // scores
            fread(table->scores, sizeof(float), table->header.player_cnt * table->header.judge_cnt, file);

            table->isFileLoad = true;
            fclose(file);
        }
    }
}

// 表获取
struct Table* Core_TableGet(int idx) {
    
    for (int i = 0; i < database.table_cnt; i++) {
        if (database.tables[i].header.idx == idx)
            return &database.tables[i];
    }
    return NULL;
}

// 表存在性检查
bool Core_TableExist(int idx) {
    
    for (int i = 0; i < database.table_cnt; i++) {
        if (database.tables[i].header.idx == idx) return true;
    }
    return false;
}

// 选手存在性检查
// -2 为数据不合法，-1 为编号不存在，其余为存在时数组内下标
int  Core_TablePlayerExist(struct Table* table, int player_idx) {

    if (player_idx < 0) return -2;
    for (int i = 0; i < table->header.player_cnt; i++) {
        if (table->players[i] == player_idx) return i;
    }
    return -1;
}

// 表的裁判人数合法性检查
bool Core_TableJudgeValid(int judge) {
    return judge > 2;
}

// 表元数据输出
void Core_TableMetaOutput(struct Table* table) {

    // lens
    int len_table = 0, len_player = 0, len_judge = 0;               // 字段输出长度
    len_table  = max(5, Cli_IntLength(table->header.idx));          // "Table"
    len_player = max(7, Cli_IntLength(table->header.player_cnt));   // "Players"
    len_judge  = max(6, Cli_IntLength(table->header.judge_cnt));    // "Judges"

    OUTPUT_CYAN("[ 表元数据 ]\n");
    printf("| %-*s | %-*s | %-*s |\n",
        len_table,  "Table",
        len_player, "Players",
        len_judge,  "Judges");

    if (table->header.idx == -1) printf("| %-*s ", len_table, "NULL");
    else printf("| %-*d ", len_table, table->header.idx);

    if (table->header.player_cnt == -1) printf("| %-*s ", len_player, "NULL");
    else printf("| %-*d ", len_player, table->header.player_cnt);

    if (table->header.judge_cnt == -1) printf("| %-*s |\n", len_judge, "NULL");
    else printf("| %-*d |\n", len_judge, table->header.judge_cnt);
}

// 表输出
void Core_TableOutput(struct Table* table,
    int order_mode, int predicate_mode, float predicate_x, float predicate_y)
{
    OUTPUT_CYAN("[ 表 ]\n");
    if (table->header.player_cnt) {

        int player_cnt = table->header.player_cnt;
        int judge_cnt  = table->header.judge_cnt;

        // 构建数据分析字段
        int alys_fields_cnt = 1;                                        // 分析字段数量
        int alys_cnt = alys_fields_cnt * player_cnt;                    // 所有分析字段数量
        float* alys = (float*)malloc(sizeof(float) * alys_cnt);
        memset(alys, 0, sizeof(float) * alys_cnt);
        {
            for (int i = 0; i < player_cnt; i++) {

                float  xres = 0.0f;
                int    imax = -1, imin = -1;                            // 最大最小值的下标（可能相同）
                float* scores = &table->scores[i * judge_cnt];          // 选手的多项成绩数组首地址

                // 建表时已要求 Judge 数量 > 2
                if (scores[0] == scores[1]) {
                    imax = 0, imin = 1;         // 避免 max 和 min 标记为同一个数字
                }

                for (int j = 0; j < judge_cnt; j++) {
                    float score = scores[j];
                    xres += score;
                    if (imax == -1 || score > scores[imax]) imax = j;
                    if (imin == -1 || score < scores[imin]) imin = j;
                }

                if (imax >= 0 && imin >= 0) {
                    if (imax == imin) xres -= scores[imax];
                    else xres -= scores[imax] + scores[imin];
                }

                alys[i * alys_fields_cnt + 0] = xres / (judge_cnt - 2);
            }
        }

        // 构建输出顺序
        int seq_cnt = 0;
        struct Kv_Player_Score* seq = (struct Kv_Player_Score*)malloc(sizeof(struct Kv_Player_Score) * player_cnt);
        {
            // predicate and fill
            for (int i = 0; i < player_cnt; i++) {

                int   player = table->players[i];
                float score = alys[i * alys_fields_cnt + 0];

                if (predicate_mode == PLAYERPREDICATE_ID) {
                    if (player < predicate_x || player > predicate_y) continue;
                }
                else if (predicate_mode == PLAYERPREDICATE_SCORE) {
                    if (score < predicate_x || score > predicate_y) continue;
                }

                struct Kv_Player_Score kv = { .idx = i, .player = player, .score = score };
                seq[seq_cnt++] = kv;
            }

            // order
            if (order_mode == PLAYERORDER_ID_ASC) {
                qsort(seq, seq_cnt, sizeof(struct Kv_Player_Score), Kv_Id_Score_AscId);
            }
            else if (order_mode == PLAYERORDER_ID_DESC) {
                qsort(seq, seq_cnt, sizeof(struct Kv_Player_Score), Kv_Id_Score_DescId);
            }
            else if (order_mode == PLAYERORDER_SCORE_ASC) {
                qsort(seq, seq_cnt, sizeof(struct Kv_Player_Score), Kv_Id_Score_AscScore);
            }
            else if (order_mode == PLAYERORDER_SCORE_DESC) {
                qsort(seq, seq_cnt, sizeof(struct Kv_Player_Score), Kv_Id_Score_DescScore);
            }
        }

        // 计算列的最大输出长度
        int  lens_cnt = 1 + judge_cnt + alys_fields_cnt;
        int* lens = (int*)malloc(sizeof(int) * lens_cnt);
        {
            // header
            lens[0] = 7;                                            // "Players"
            for (int i = 0; i < judge_cnt; i++) {
                lens[i + 1] = 6 + Cli_IntLength(i + 1);             // "Judge 1"
            }
            lens[judge_cnt] = 5;                                    // "Score"

            // data
            for (int i = 0; i < seq_cnt; i++) {

                int player_idx = seq[i].idx;

                // Player
                lens[0] = max(lens[0], Cli_IntLength(table->players[player_idx]));

                // Judge
                for (int j = 0; j < judge_cnt; j++) {
                    lens[j + 1] = max(lens[j + 1],
                        Cli_FloatLength(table->scores[player_idx * judge_cnt + j], OUTPUT_PREC));
                }

                // Score
                lens[judge_cnt] = max(
                    lens[judge_cnt], Cli_FloatLength(alys[player_idx * alys_fields_cnt + 0], OUTPUT_PREC));
            }
        }

        // 输出
        {
            // header
            {
                // Players
                printf("| %-*s |", lens[0], "Players");

                // Judges
                char str_temp[STRING_MAXLEN] = { '\0' };
                for (int i = 0; i < judge_cnt; i++) {
                    snprintf(str_temp, STRING_MAXLEN, "Judge %d", i + 1);
                    printf(" %-*s |", lens[i + 1], str_temp);
                }

                // Score
                printf(" %-*s |", lens[judge_cnt], "Score");
            }

            printf("\n");

            // data
            {
                for (int i = 0; i < seq_cnt; i++) {

                    int player_idx = seq[i].idx;

                    // Player
                    printf("| %-*d |", lens[0], table->players[player_idx]);

                    // Judge
                    for (int j = 0; j < judge_cnt; j++) {
                        printf(" %-*.*f |", lens[j + 1], OUTPUT_PREC,
                            table->scores[player_idx * table->header.judge_cnt + j]);
                    }

                    // Score
                    printf(" %-*.*f |", lens[judge_cnt],
                        OUTPUT_PREC, alys[player_idx * alys_fields_cnt + 0]);

                    printf("\n");
                }
            }
        }

        free(seq);
        free(alys);
        free(lens);
    }
    else {
        OUTPUT_YELLOW("| 表内无任何数据 |\n");
    }
}

// 启动
void Core_Open() {

    WIN32_FIND_DATA find;
    HANDLE handle = FindFirstFile(FOLDER_TABLES_ALL, &find);

    // 文件夹不存在
    if (handle == INVALID_HANDLE_VALUE) {
        CreateDirectory(FOLDER_DATA, NULL);
        CreateDirectory(FOLDER_TABLES, NULL);
        return;
    }

    // 读取
    do {
        char path[STRING_MAXLEN] = { '\0' };
        snprintf(path, STRING_MAXLEN, "%s/%s", FOLDER_TABLES, find.cFileName);
        FILE* file = fopen(path, "rb");

        if (file) {
            struct Table t = {
                .isFileLoad = false, .isFileWrite = true,    // 尚未完整读取，认定为已完成写入
                .players = NULL, .scores = NULL
            };
            fread(&t.header, sizeof(struct TableFileHeader), 1, file);
            fclose(file);

            Core_DatabaseExpand(1);
            database.tables[database.table_cnt - 1] = t;
        }
    } while (FindNextFile(handle, &find));
}

// 关闭
void Core_Close() {

    // 在文件曾完整读取并尚未写入文件时
    for (int i = 0; i < database.table_cnt; i++) {
        if (database.tables[i].isFileLoad == true &&
            database.tables[i].isFileWrite == false)
        {
            char path[STRING_MAXLEN] = { '\0' };
            snprintf(path, STRING_MAXLEN, "%s/%d%s",
                FOLDER_TABLES, database.tables[i].header.idx, FILE_TABLE_SUFFIX);
            FILE* file = fopen(path, "wb");
            if (file) {

                // header
                fwrite(&database.tables[i].header, sizeof(struct TableFileHeader), 1, file);

                // players
                if (database.tables[i].players == NULL) continue;
                fwrite(database.tables[i].players, sizeof(int),
                    database.tables[i].header.player_cnt,
                    file);

                // scores
                if (database.tables[i].scores == NULL) continue;
                fwrite(database.tables[i].scores, sizeof(float),
                    database.tables[i].header.player_cnt * database.tables[i].header.judge_cnt,
                    file);

                database.tables[i].isFileWrite = true;
                fclose(file);
            }
        }
    }
}
