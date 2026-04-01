
struct Table {
    int     cnt_player;
    int     cnt_judge;
    float** scores;
};

struct Database {
    int           cnt_table;
    struct Table* tables;
};

extern struct Database database;

void Core_Main(char cmd[]);
