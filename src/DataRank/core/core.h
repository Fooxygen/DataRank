
#pragma once
#include "../public.h"

struct Kv_Player_Score {
    int   idx;
    int   player;
    float score;
};

struct TableFileHeader {
    int idx;
    int player_cnt;
    int judge_cnt;
};

struct Table {
    struct TableFileHeader header;
    int*   players;                 // 所有选手唯一编号
    float* scores;                  // 所有选手成绩（按一维数组排布，每 cnt_judge 项为一个选手的成绩）

    bool   isFileLoad;              // 是否将文件数据全部读取到内存
    bool   isFileEqualMemory;       // 是否将内存数据全部写入文件中
};

struct Database {
    int           table_cnt;
    struct Table* tables;
};

extern struct Database database;

void            Core_DatabaseExpand(int cnt);
void            Core_TableExpand(struct Table* table, int player_cnt, int judge_cnt);
void            Core_TableFileLoad(struct Table* table);

struct Table*   Core_TableGet(int idx);
bool            Core_TableExist(int idx);
int             Core_TablePlayerExist(struct Table* table, int player_idx);
bool            Core_TableJudgeValid(int judge);

void            Core_TableMetaOutput(struct Table* table);
void            Core_TableOutput(struct Table* table,
                                 int order_mode, int predicate_mode, float predicate_x, float predicate_y);

void            Core_Open();
void            Core_Close();
