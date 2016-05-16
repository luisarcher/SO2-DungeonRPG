#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Client.h"

//id msg
#define SETNAME		600
#define STARTGAME	601
#define QUITGAME	604
#define MOVEUP		672
#define MOVEDOWN	680
#define MOVELEFT	675
#define MOVERIGHT	677
#define BUFFERSIZE 256

#define PLAYER_LOS 10

bool iniciado;

typedef struct CLIENTREQUEST {
	int command;
	TCHAR msg[BUFFERSIZE];
} ClientRequest;

typedef struct SERVERRESPONSE {
	int matriz[PLAYER_LOS][PLAYER_LOS];
	TCHAR msg[BUFFERSIZE];
} ServerResponse;


void EscreveMensagem(HANDLE pipe, ClientRequest req) {
	DWORD n;
	BOOL ret;
	TCHAR buf[256];

	

	/*_tprintf(TEXT("[CLIENTE] Nome: "));
	_fgetts(req.msg, 256, stdin);*/

	//_tcscpy(req.msg, TEXT("rebeca")); //nome
	//_tcscpy(req.msg, '\0');


	

	if (!WriteFile(pipe, &req, sizeof(ClientRequest), &n, NULL)) {
		_tprintf(TEXT("Erro"));
	}
	else
		tcout << TEXT("[CLIENTE]Escrevi...\n"); 



	
	
}

void LerMensagem(HANDLE pipe) {
	BOOL ret;
	ServerResponse resp;
	DWORD n;
	
	ret = ReadFile(pipe, &resp, sizeof(ServerResponse), &n, NULL);
	//buf[n / sizeof(TCHAR)] = '\0';
	/*if (!ret || !n)
		break;*/
	tcout << TEXT("[CLIENTE] Recebi ") << n << TEXT(" bytes: \'") << resp.msg << TEXT("\'... (ReadFile)\n");
}

DWORD WINAPI EsperaComando(LPVOID param) {
	HANDLE pipe = (HANDLE)param;
	TCHAR buf[256];
	DWORD n;
	BOOL ret;
	ClientRequest req;
	ServerResponse resp;
	while (1) {
		memset(req.msg, '\0', sizeof(TCHAR));
		_tprintf(TEXT("[CLIENTE] Frase: "));
		_fgetts(req.msg, 256, stdin);
		req.command = (int)SETNAME;
		EscreveMensagem(pipe, req);
		LerMensagem(pipe);
		
	}
	return 0;
}


DWORD WINAPI LerBroadcast(LPVOID param) {
	HANDLE pipe = (HANDLE)param;
	BOOL ret;
	ServerResponse resp;
	DWORD n;
	clrscr();
	while (1)
	{
		

		ret = ReadFile(pipe, &resp, sizeof(ServerResponse), &n, NULL);
		//buf[n / sizeof(TCHAR)] = '\0';
		if (iniciado == true)
		{
			if (ret || n)
				for (int i = 0; i < 10; i++)
				{
					for (int j = 0; j < 10; j++)
					{
						//tcout << resp.matriz[i][j];
						//system("cls");
						
						MostraLOS(resp.matriz);
					}
				}
		}
		
		if (resp.msg != TEXT('\0')) {
			tcout << TEXT("[CLIENTE] Recebi ") << n << TEXT(" bytes: \'") << resp.msg << TEXT("\'... (ReadFile)\n");
		}
		tcout << TEXT("\n\n>>\n");
	}
	return 0;
}

//enviar escape e mostrar menu escape

void enviaTecla(int k, HANDLE pipe) {
	ClientRequest req;
	int kp, seta = 0;
	switch (k)
	{
	case 1:
	{
		//_fgetts(req.msg, 256, stdin);
		memset(req.msg, '\0', sizeof(TCHAR));
		_tcscpy(req.msg, TEXT("UP\n"));
		req.command = (int)MOVEUP;
		EscreveMensagem(pipe, req);
		//LerMensagem(pipe);
		break;
	}
	case 2:
	{
		//_fgetts(req.msg, 256, stdin);
		memset(req.msg, '\0', sizeof(TCHAR));
		_tcscpy(req.msg, TEXT("Down\n"));
		req.command = (int)MOVEDOWN;
		EscreveMensagem(pipe, req);
		//LerMensagem(pipe);
		break;
	}
	case 3:
	{
		//_fgetts(req.msg, 256, stdin);
		memset(req.msg, '\0', sizeof(TCHAR));
		_tcscpy(req.msg, TEXT("Left\n"));
		req.command = (int)MOVELEFT;
		EscreveMensagem(pipe, req);
		//LerMensagem(pipe);
		break;
	}
	case 4:
	{
		//_fgetts(req.msg, 256, stdin);
		memset(req.msg, '\0', sizeof(TCHAR));
		_tcscpy(req.msg, TEXT("Right\n"));
		req.command = (int)MOVERIGHT;
		EscreveMensagem(pipe, req);
		//LerMensagem(pipe);
		break;
	}
	case 7:
		iniciado = false;
		while (iniciado == false)
		{
			kp = Getch();
			switch (kp)
			{
			case 1:
				if (seta == 0)
				{
					seta = 2;
					MenuEscape(seta);
				}
				else
				{
					seta--;
					MenuEscape(seta);
				}
				break;
			case 2:
				if (seta == 1)
				{
					seta = 0;
					MenuEscape(seta);
				}
				else
				{
					seta++;
					MenuEscape(seta);
				}
				break;
			default:
				//nope
				break;
			}
			
		
		}
		break;
	
	case 5:
		//apanha pedras
		break;
	default:
		break;
	}

}



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

int _tmain(int argc, LPTSTR argv[]) {
	TCHAR buf[256];
	HANDLE hPipe,hPipeJogo;
	//int i = 0;
	//BOOL iniciado = FALSE;
	BOOL ret;
	DWORD n;
	HANDLE hThread;
	ClientRequest req;
#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif
	iniciado = false;
	//Ligar ao pipe (PIPE_NAME)
	tcout << TEXT("[CLIENTE]Esperar pelo pipe \'") << PIPE_NAME << TEXT("\'(WaitNamedPipe)\n");
	if (!WaitNamedPipe(PIPE_NAME, NMPWAIT_WAIT_FOREVER)) {
		tcout << TEXT("[ERRO] Ligar ao pipe \'") << PIPE_NAME << TEXT("\'... (WaitNamedPipe)\n");
		system("pause");
		exit(-1);
	}

	tcout << TEXT("[CLIENTE] Ligação ao servidor... (CreateFile)\n");
	if ((hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == NULL) {
		tcerr << TEXT("[ERRO] Ligar ao pipe \'") << PIPE_NAME << TEXT("\'... (CreateFile)\n");
		system("pause");
		exit(-1);
	}

	tcout << TEXT("[CLIENTE] Ligação ao servidor... (CreateFile)\n");
	if ((hPipeJogo = CreateFile(PIPE_NAME_JOGO, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == NULL) {
		tcerr << TEXT("[ERRO] Ligar ao pipe \'") << PIPE_NAME_JOGO << TEXT("\'... (CreateFile)\n");
		system("pause");
		exit(-1);
	}

	tcout << TEXT("[CLIENTE]Liguei-me...\n");
	
	
	//hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EsperaComando, (LPVOID)hPipe, 0, NULL);

	//WaitForSingleObject(hThread, INFINITE);

	
	int kp, seta = 0;
	MenuInicial(seta);
	while (iniciado = TRUE)
	{
		
		kp = Getch();
		switch (kp)
		{
		case 1:
			if (seta == 0)
			{
				seta = 2;
				MenuInicial(seta);
			}
			else
			{
				seta--;
				MenuInicial(seta);
			}
			break;
		case 2:
			if (seta == 2)
			{
				seta = 0;
				MenuInicial(seta);
			}
			else
			{
				seta++;
				MenuInicial(seta);
			}
			break;
		case 6:
			//caso 1: posiciona o jogador no server, pede LOS mostra HUD
			
			
			if (seta == 0)
			{
				int k = 0;
				
				ClientRequest req;
				req.command = 601;
				_tcscpy(req.msg, TEXT("Inicia..\n"));
				//envia o codigo de inicio
				EscreveMensagem(hPipe, req);
				//Lê o "iniciaste"
				LerMensagem(hPipe);
				system("pause");
				/*entre estas funções é preciso esperar pelo setup dos players*/
				//Broadcast da posiçao
				hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LerBroadcast, (LPVOID)hPipeJogo, 0, NULL);
				
				//envia movimentos agora
				//HANDLE hMove = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Movimento, (LPVOID)hPipeJogo, 0, NULL);
				while (1)
				{
					clrscr();
					k = Getch();
					enviaTecla(k,hPipe);
				}
				//system("pause");
			}
			else if (seta == 1)
			{
				memset(req.msg, '\0', sizeof(TCHAR));
				_tprintf(TEXT("[CLIENTE] Frase: "));
				_fgetts(req.msg, 256, stdin);
				req.command = (int)SETNAME;
				EscreveMensagem(hPipe, req);
				LerMensagem(hPipe);
			}
			else
				//sai do jogo
				//CloseHandle(hPipe);
				break;
			break;
		default:
			break;
		}
	}

	//CloseHandle(hPipe);

	
	return 0;
}


//2315