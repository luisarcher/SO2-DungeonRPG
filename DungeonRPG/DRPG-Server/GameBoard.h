#ifndef GAMEBOARD_H_INCLUDED
#define GAMEBOARD_H_INCLUDED

#include "Player.h"

typedef struct GAMEBOARD GameBoard;
struct GAMEBOARD {
	int gameBoard[GAMEBOARDSIZE][GAMEBOARDSIZE];
	BOOL init;
};

GameBoard NovoGameBoard();
GameBoard LerGameBoard();

void DistribuirItems();

BOOL hasPlayerIn(int x, int y);
BOOL hasMonsterAndPlayerIn(int x, int y);
BOOL hasObjectIn(int x, int y);
BOOL hasWallIn(int x, int y);

//extern GameBoard gGameBoard;
extern GameBoard * shGameBoard;
#endif