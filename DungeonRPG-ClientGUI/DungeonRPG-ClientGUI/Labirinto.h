#pragma once
#include "Common.h"

typedef struct LABIRINTO {
	int labirinto[LABIRINTOSIZE][LABIRINTOSIZE];
	BOOL init;
} Labirinto;

//Opacos
BOOL isWall(int pos);

//Com transparência
BOOL isObject(int pos);

BOOL isPlayer(int pos);

BOOL isMonsterAndPlayer(int pos);