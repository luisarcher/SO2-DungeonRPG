#include "Client.h"

TCHAR broadcastMessage[BUFFERSIZE];
BOOL iniciado = FALSE;
BOOL fim = FALSE;

void EscreveMensagem(HANDLE pipe, ClientRequest req) {
	DWORD n;
	if (!WriteFile(pipe, &req, sizeof(ClientRequest), &n, NULL)) {
		_tprintf(TEXT("[CLIENTE] Erro ao enviar mensagem ao servidor!\n"));
		system("pause");
		exit(-1);
	}
	//else _tprintf(TEXT("[CLIENTE]Pedido enviado [%d-%s]\n"),req.command,req.msg); 
}

void LerMensagem(HANDLE pipe, TCHAR * serverResponse) {
	DWORD n;
	if (!ReadFile(pipe, serverResponse, sizeof(TCHAR) * BUFFERSIZE, &n, NULL)) {
		_tperror(TEXT("[CLIENTE] Erro ao ler mensagem do servidor!\n"));
		system("pause");
		exit(-1);
	}
}

DWORD WINAPI LerBroadcast(LPVOID param) {
	HANDLE pipe = (HANDLE)param;
	ServerResponse resp;
	BOOL ret;
	
	DWORD n;

	while (!fim)
	{
		ret = ReadFile(pipe, &resp, sizeof(ServerResponse), &n, NULL);
		if (iniciado == TRUE && (ret || n))
			MostraLOS(resp.matriz);
		if (resp.msg[0] != TEXT('\0')) {
			//_tprintf(TEXT("[CLIENTE] Recebi %d bytes: \'%s\'... (ReadFile)\n"), n, resp.msg);
			_tcscpy(broadcastMessage,resp.msg);
		}
	}
	return 0;
}

void enviaTecla(int k, HANDLE pipe) {
	ClientRequest req;
	int kp, seta = 0;
	switch (k)
	{
	case KEY_UP:
	{
		//_fgetts(req.msg, 256, stdin);
		//memset(req.msg, '\0', sizeof(TCHAR) * BUFFERSIZE);
		_tcscpy(req.msg, TEXT("UP"));
		req.command = (int)MOVEUP;
		EscreveMensagem(pipe, req);
		//LerMensagem(pipe);
		break;
	}
	case KEY_DOWN:
	{
		//_fgetts(req.msg, 256, stdin);
		//memset(req.msg, '\0', sizeof(TCHAR));
		_tcscpy(req.msg, TEXT("Down"));
		req.command = (int)MOVEDOWN;
		EscreveMensagem(pipe, req);
		//LerMensagem(pipe);
		break;
	}
	case KEY_LEFT:
	{
		//_fgetts(req.msg, 256, stdin);
		memset(req.msg, '\0', sizeof(TCHAR));
		_tcscpy(req.msg, TEXT("Left"));
		req.command = (int)MOVELEFT;
		EscreveMensagem(pipe, req);
		//LerMensagem(pipe);
		break;
	}
	case KEY_RIGHT:
	{
		//_fgetts(req.msg, 256, stdin);
		memset(req.msg, '\0', sizeof(TCHAR));
		_tcscpy(req.msg, TEXT("Right"));
		req.command = (int)MOVERIGHT;
		EscreveMensagem(pipe, req);
		//LerMensagem(pipe);
		break;
	}
	case KEY_ESCAPE:
	{
		iniciado = FALSE;

		clrscr();
		seta = 0;

		gotoxy(0, 0);
		MenuEscape(seta);

		while (iniciado == FALSE)
		{
			kp = Getch();
			switch (kp)
			{
			case KEY_UP:
				if (seta == 0)
				{
					seta = 1;
					MenuEscape(seta);
				}
				else
				{
					seta--;
					MenuEscape(seta);
				}
				break;
			case KEY_DOWN:
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
			case KEY_ENTER:
				if (seta == 0)
				{
					iniciado = TRUE;
				}
				else if (seta == 1)
				{
					MenuInicial(0);
				}
				//break;
			default:
				//nope
				break;
			}
		}
		break;
	}
	case KEY_SPACE:
	{
		memset(req.msg, '\0', sizeof(TCHAR));
		_tcscpy(req.msg, TEXT("Activa Pedras\n"));
		req.command = (int)SWITCH_STONE_AUTOHIT;
		EscreveMensagem(pipe, req);

		break;
	}
	default:
		break;
	}
}