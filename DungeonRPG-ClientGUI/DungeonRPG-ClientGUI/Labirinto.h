#pragma once
#include "Common.h"

typedef struct LABIRINTO {
	int labirinto[LABIRINTOSIZE][LABIRINTOSIZE];
	BOOL init;
} Labirinto;

BOOL isWall(int pos);

BOOL isObject(int pos);

BOOL isMonster(int pos);

BOOL isPlayer(int pos);

BOOL isMonsterAndPlayerIn(int pos);