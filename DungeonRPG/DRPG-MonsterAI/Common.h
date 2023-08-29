#pragma once

#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

#define GAMEBOARDSIZE 70
#define MONSTER_MAT_SIZE 20

extern BOOL fim;

typedef struct {
	int gameBoard[GAMEBOARDSIZE][GAMEBOARDSIZE];
	BOOL init;
} GameBoard;

extern HANDLE gMutexGameBoard;
extern GameBoard * shGameBoard;

extern HANDLE ghUpdateGameClientEvent;

/*****  Items da matriz | Chart de ints  *****/
#define EMPTY -1
#define FOG_OF_WAR -2

//Players 0~19
#define PLAYER_START_INDEX 0
#define PLAYER_END_INDEX 19

//Paredes 20~29
#define WALL_START_INDEX 20
#define WALL_END_INDEX 29

//Items 30~49 | 500+
#define ITEM_START_INDEX 30
#define PEDRAS 500		//+1 ataque quando usada
//pedras 50(quant), 500+ , ex 503 -> 3 pedras numa posição
#define ITEM_END_INDEX 49

#define MONSTER_START_INDEX 51
#define MAX_MONSTERS 48
#define MONSTER_END_INDEX (int)((int)MONSTER_START_INDEX+(int)MAX_MONSTERS)