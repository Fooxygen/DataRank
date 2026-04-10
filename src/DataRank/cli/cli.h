
#pragma once
#include "../public.h"

bool Cli_Main();
bool Cli_Help();

void Cli_Clear();
bool Cli_SelectReturn();
bool Cli_Input(char cmd[], void(*callback)());

int  Cli_IntLength(int x);
int  Cli_FloatLength(float x, int prec);