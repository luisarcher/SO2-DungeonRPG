#ifndef DRPG_SERVER_SERVER_H_
#define DRPG_SERVER_SERVER_H_
#include "Common.h"
#include "Player.h"

#define PIPE_NAME TEXT("\\\\.\\pipe\\DRPG")
#define PIPE_NAME_JOGO TEXT("\\\\.\\pipe\\DRPG-Jogo")

//Client Commands
#define SETNAME		600
#define STARTGAME	601
#define QUITGAME	604
#define GAMESTATUS	610
#define MOVEUP		672
#define MOVEDOWN	680
#define MOVELEFT	675
#define MOVERIGHT	677
#define SWITCH_STONE_AUTOHIT 650

typedef struct {
	int command;
	TCHAR msg[BUFFERSIZE];
} ClientRequest;

typedef struct {
	int playerLOS[PLAYER_LOS][PLAYER_LOS];
	TCHAR msg[BUFFERSIZE];
	PlayerInformation playerInfo;
} ServerResponse;

DWORD WINAPI ListenForConnections	(LPVOID param);
DWORD WINAPI HandleClientConnection	(LPVOID param);
DWORD WINAPI BroadcastServerData	(LPVOID param);
DWORD WINAPI GameTimer				(LPVOID param);
DWORD WINAPI GameEvents				(LPVOID param);

int  GetActivePlayers();
void StartGame();

//void CopyGameStateToSharedMemory();
void DisconnectClientThreads();

extern TCHAR broadcastMessage[BUFFERSIZE];

#endif // DRPG_SERVER_SERVER_H_