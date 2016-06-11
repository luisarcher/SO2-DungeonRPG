#pragma once
#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include "lib\ClientDLL.h"

#define N_BITMAPS			10
#define LABIRINTOSIZE		70
#define TILE_SZ				64
#define BOARD_TOP_MARGIN	25
#define BOARD_LEFT_MARGIN	25

/* === GAME COMMANDS === */
#define SETNAME		600
#define STARTGAME	601
#define QUITGAME	604
#define GAMESTATUS	610
#define MOVEUP		672
#define MOVEDOWN	680
#define MOVELEFT	675
#define MOVERIGHT	677
#define SWITCH_STONE_AUTOHIT 650

/*****  Items da matriz | Chart de ints  *****/
#define EMPTY -1
#define FOG_OF_WAR -2

//Players 0~19
#define PLAYER_START_INDEX 0
#define PLAYER_END_INDEX 19

//Paredes 20~29
#define WALL_START_INDEX 20
#define BRICK 20
#define WALL_END_INDEX 29

//Items 30~49 | 500+
#define ITEM_START_INDEX 30
#define VITAMINA	30	//+1 HP (Up to 200% base hp (200))
#define ORANGE_BULL 31	//+3 HP (Up to 200% base hp (200))
#define REB_CAFEINA 32	//-2 Lentidão (1min, não stackam)
#define PEDRAS 500		//+1 ataque quando usada
//pedras 50(quant), 500+ , ex 503 -> 3 pedras numa posição
#define ITEM_END_INDEX 49

#define MONSTER_START_INDEX 51
#define MONSTER_A	52	//Distraido
#define MONSTER_B	51	//Agressivo

//Common
extern HINSTANCE ghInstance;

//Connect
extern HANDLE hPipe;
extern HANDLE hPipeJogo;
extern HANDLE hThreadBroadcastReceiver;

extern int connected;
extern int started;