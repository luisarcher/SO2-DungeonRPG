#pragma once
#include "Common.h"

// TODO: Test if we need this. As the map is being painted directly from the response from the server.
typedef struct LABIRINTO {
	int labirinto[GAMEMAPSIZE][GAMEMAPSIZE];
	BOOL init;
} Labirinto;

// Opaque
BOOL isWall(int pos);

// Transparent
BOOL isObject(int pos);

BOOL isPlayer(int pos);

BOOL isMonsterAndPlayer(int pos);