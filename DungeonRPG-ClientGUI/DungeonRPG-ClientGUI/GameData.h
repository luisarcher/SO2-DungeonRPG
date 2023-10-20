#pragma once
#include "Common.h"

/* Bitmap order in array*/
#define MONSTER_A_I		0
#define MONSTER_B_I		1
#define GRASS_I			2
#define BRICK_I			3
#define GRANITE_I		4
#define PLAYER_I		5
#define VITAMINA_I		6
#define ORANGE_BULL_I	7
#define REB_CAFEINA_I	8
#define STONES_I		9

extern TCHAR * errorHandlingPipe[3];
extern TCHAR * _bitmaps[N_BITMAPS];

DWORD WINAPI UpdateGameView(LPVOID lpParam);