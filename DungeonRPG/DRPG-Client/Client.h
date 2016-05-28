#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

//Comandos do utilizador
#define SETNAME		600
#define STARTGAME	601
#define QUITGAME	604
#define MOVEUP		672
#define MOVEDOWN	680
#define MOVELEFT	675
#define MOVERIGHT	677
#define SWITCH_STONE_AUTOHIT 650
#define BUFFERSIZE 256
#define PLAYER_LOS 10

#define PIPE_NAME TEXT("\\\\.\\pipe\\DRPG")
#define PIPE_NAME_JOGO TEXT("\\\\.\\pipe\\DRPG-Jogo")

typedef struct CLIENTREQUEST {
	int command;
	TCHAR msg[BUFFERSIZE];
} ClientRequest;

typedef struct SERVERRESPONSE {
	int matriz[PLAYER_LOS][PLAYER_LOS];
	TCHAR msg[BUFFERSIZE];
} ServerResponse;

extern BOOL fim;
extern ServerResponse resp;

void InicializarPipes(HANDLE * hPipe, HANDLE * hPipeJogo);
void LerMensagem(HANDLE pipe, TCHAR * serverResponse);
void EscreveMensagem(HANDLE pipe, ClientRequest req);
DWORD WINAPI LerBroadcast(LPVOID param);
void FecharHandles(HANDLE * hPipe, HANDLE * hPipeJogo);
#endif