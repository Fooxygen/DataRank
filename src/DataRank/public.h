
#pragma once
#pragma warning(disable: 6031)

#define _CRT_SECURE_NO_WARNINGS

#define STRING_MAXLEN				128
#define LIST_MAXSIZE				128
#define FOLDER_DATA					"data"
#define FOLDER_TABLES				"data/tables"
#define FOLDER_TABLES_ALL			"data/tables/*"
#define FILE_TABLE_SUFFIX			".table"

#define OUTPUT_WHITE_STR			"\033[0m"
#define OUTPUT_RED_STR				"\033[91m"
#define OUTPUT_YELLOW_STR			"\033[33m"
#define OUTPUT_GREEN_STR			"\033[32m"
#define OUTPUT_CYAN_STR				"\033[36m"
#define OUTPUT_RED(s)				\
	printf(OUTPUT_RED_STR);			\
	printf(s);						\
	printf(OUTPUT_WHITE_STR);
#define OUTPUT_YELLOW(s)			\
	printf(OUTPUT_YELLOW_STR);		\
	printf(s);						\
	printf(OUTPUT_WHITE_STR);
#define OUTPUT_GREEN(s)				\
	printf(OUTPUT_GREEN_STR);		\
	printf(s);						\
	printf(OUTPUT_WHITE_STR);
#define OUTPUT_CYAN(s)				\
	printf(OUTPUT_CYAN_STR);		\
	printf(s);						\
	printf(OUTPUT_WHITE_STR);

#define OUTPUT_PREC					2

#define PLAYERORDER_EMPTY			0
#define PLAYERORDER_ID_ASC			1
#define PLAYERORDER_ID_DESC			2
#define PLAYERORDER_SCORE_ASC		3
#define PLAYERORDER_SCORE_DESC		4

#define PLAYERPREDICATE_EMPTY		0
#define PLAYERPREDICATE_ID			1
#define PLAYERPREDICATE_SCORE		2

#include <stdbool.h>
