#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED
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
	int matriz[PLAYER_LOS][PLAYER_LOS];
	TCHAR msg[BUFFERSIZE];
	PlayerInformation playerInfo;
} ServerResponse;

DWORD WINAPI ListenForConnections(LPVOID param);		//Listen
DWORD WINAPI HandleClientConnection(LPVOID param);
DWORD WINAPI ActualizaClientes(LPVOID param);
DWORD WINAPI GameTimer(LPVOID param);
DWORD WINAPI GameEvents(LPVOID param);

int activePlayers();
void StartGame();

//void CopyGameStateToSharedMemory();
void DesligarThreadsDeCliente();

extern TCHAR broadcastMessage[BUFFERSIZE];
#endif