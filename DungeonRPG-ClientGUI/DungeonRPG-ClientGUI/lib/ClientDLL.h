#pragma once
#ifndef CLIENT_H
#define CLIENT_H
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
//#include <string>

#define TAM 256

#ifdef DLL_EXPORTS
#define DLL_IMP_API __declspec(dllexport)
#else
#define DLL_IMP_API __declspec(dllimport)
#endif

/*#ifdef UNICODE
#define tstring wstring
#else
#define tstring string
#endif*/

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

//#define PIPE_NAME TEXT("\\\\.\\pipe\\DRPG")
//#define PIPE_NAME_JOGO TEXT("\\\\.\\pipe\\DRPG-Jogo")

#define PIPE_NAME_SUFFIX TEXT("\\pipe\\DRPG")
#define PIPE_NAME_JOGO_SUFFIX TEXT("\\pipe\\DRPG-Jogo")

typedef struct {
	int command;
	TCHAR msg[BUFFERSIZE];
} ClientRequest;

typedef struct {
	int matriz[PLAYER_LOS][PLAYER_LOS];
	TCHAR msg[BUFFERSIZE];
} ServerResponse;

extern DLL_IMP_API BOOL fim;
extern DLL_IMP_API ServerResponse resp;

DLL_IMP_API int InicializarPipes(HANDLE * hPipe, HANDLE * hPipeJogo, TCHAR * ipServ);
DLL_IMP_API DWORD LerMensagem(HANDLE pipe, TCHAR * serverResponse);
DLL_IMP_API DWORD EscreveMensagem(HANDLE pipe, ClientRequest req);
DLL_IMP_API DWORD WINAPI LerBroadcast(LPVOID param);
DLL_IMP_API void FecharHandles(HANDLE * hPipe, HANDLE * hPipeJogo);

#endif