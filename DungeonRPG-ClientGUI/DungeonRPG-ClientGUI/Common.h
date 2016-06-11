#pragma once
#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include "lib\ClientDLL.h"

#define N_BITMAPS			7
#define LABIRINTOSIZE		70
#define TILE_SZ				64
#define BOARD_TOP_MARGIN	25
#define BOARD_LEFT_MARGIN	25

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

//Common
extern HINSTANCE ghInstance;

//Connect
extern HANDLE hPipe;
extern HANDLE hPipeJogo;
extern HANDLE hThreadBroadcastReceiver;

extern TCHAR receivedMSG[BUFFERSIZE];
extern int connected;
extern int started;