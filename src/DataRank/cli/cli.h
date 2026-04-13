
#pragma once
#include "../public.h"

bool Cli_Main();
bool Cli_Help();

void Cli_Clear();
void Cli_ClearLines(int n);
bool Cli_SelectReturn();
bool Cli_Input(char cmd[], bool* last_execute, void(*callback)());

int  Cli_IntLength(int x);
int  Cli_FloatLength(float x, int prec);