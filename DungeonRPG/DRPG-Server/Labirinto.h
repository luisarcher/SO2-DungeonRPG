#ifndef LABIRINTO_H_INCLUDED
#define LABIRINTO_H_INCLUDED

#include "Player.h"

typedef struct LABIRINTO Labirinto;
struct LABIRINTO {
	int labirinto[LABIRINTOSIZE][LABIRINTOSIZE];
	BOOL init;
};

Labirinto NovoLabirinto();
Labirinto LerLabirinto();

void DistribuirItems();

BOOL hasPlayerIn(int x, int y);
BOOL hasMonsterAndPlayerIn(int x, int y);
BOOL hasObjectIn(int x, int y);
BOOL hasWallIn(int x, int y);

//extern Labirinto gLabirinto;
extern Labirinto * shLabirinto;
#endif