#pragma once
#ifndef COMMON_HEADER
#define COMMON_HEADER
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

#ifdef UNICODE 
#define tcout wcout
#define tcin wcin
#define tcerr wcerr
#define tstring wstring
#else
#define tcout cout
#define tcin cin
#define tcerr cerr
#define tstring string
#endif

// ---  Game Constants
#define GAME_INSTANCE_TIME 200 //1000/15
#define MAX_CLIENTS 10
#define BUFFERSIZE 256
#define PLAYER_LOS 10	//player line of sight
#define GAMEBOARDSIZE 70

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

// - Ints da matriz
	#define EMPTY -1
	#define FOG_OF_WAR -2
	#define MONSTERS_MIN 8
	#define MONSTERS_MAX 16

	//Players 0~19
	#define PLAYER_START_INDEX 0
	#define PLAYER_END_INDEX 19
		#define PLAYER_STONE_CAP 15
		#define BASE_HP 10
		#define BASE_SLOWNESS 5

	// Walls 20~29
	#define WALL_START_INDEX 20
		#define STONEWALL 20
	#define WALL_END_INDEX 29

	// Items 30~49 | 500+
	#define ITEM_START_INDEX 30
		#define VITAMIN		30	//+1 HP (Up to +100% base hp (200))
		#define ORANGE_BULL 31	//+3 HP (Up to +100% base hp (200))
		#define REB_CAFFEINE 32	//-2 Lentidão (1min, não stackam)
		#define STONES 500		//+1 ataque quando usada
			//pedras 50(quant), 500+ , ex 503 -> 3 pedras numa posição
	#define ITEM_END_INDEX 49

	// Item Quantity
	#define QNT_VITAMINA  0.03		//3% vitaminas no mapa
	#define QNT_ORANGE_BULL 0.005	//0.5% orangeBull no mapa
	#define QNT_REB_CAFEINA 0.02	//2% rebuçadosCafeina no mapa
	#define QNT_PEDRAS 0.06			//6% Pedras no mapa
	
	#define MONSTER_START_INDEX 51
	#define MAX_MONSTERS 20
	#define MONSTER_END_INDEX (int)((int)MONSTER_START_INDEX+(int)MAX_MONSTERS)

// ---  Game Globals
extern BOOL fim;
extern BOOL start;		//game start
extern int totalConnections;

extern HANDLE gMutexGameBoard;
extern HANDLE ghGameInstanceEvent;
extern HANDLE ghUpdateGameClientEvent;

#endif
