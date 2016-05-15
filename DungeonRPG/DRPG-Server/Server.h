#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED
#include "Common.h"

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

typedef struct CLIENTREQUEST {
	int command;
	TCHAR msg[BUFFERSIZE];
} ClientRequest;

typedef struct SERVERRESPONSE {
	int matriz[PLAYER_LOS][PLAYER_LOS];
	TCHAR msg[BUFFERSIZE];
} ServerResponse;

DWORD WINAPI RecebeClientes(LPVOID param);
DWORD WINAPI AtendeCliente(LPVOID param);
DWORD WINAPI ActualizaClientes(LPVOID param);

void DesligarNamedPipes();

extern TCHAR broadcastMessage[BUFFERSIZE];
#endif