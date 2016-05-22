#include "Client.h"

BOOL iniciado = FALSE;
BOOL fim = FALSE;

void EscreveMensagem(HANDLE pipe, ClientRequest req) {
	DWORD n;
	if (!WriteFile(pipe, &req, sizeof(ClientRequest), &n, NULL)) {
		_tprintf(TEXT("[CLIENTE] Erro ao enviar mensagem ao servidor!\n"));
	}
	else
		_tprintf(TEXT("[CLIENTE]Escrevi...\n")); 
}

void LerMensagem(HANDLE pipe) {
	TCHAR buf[BUFFERSIZE];
	DWORD n;
	
	if (!ReadFile(pipe, buf, sizeof(TCHAR) * BUFFERSIZE, &n, NULL)) {
		_tperror(TEXT("[CLIENTE] Erro ao ler mensagem do servidor!\n"));
	}
	_tprintf(TEXT("[CLIENTE] Recebi %d bytes: \'%s\'... (ReadFile)\n"), n, buf);
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
		if (iniciado == TRUE)
		{
			if (ret || n)
				MostraLOS(resp.matriz);
		}
		else if (resp.msg[0] != TEXT('\0')) {
			_tprintf(TEXT("[CLIENTE] Recebi %d bytes: \'%s\'... (ReadFile)\n"), n, resp.msg);
			//_tprintf(TEXT("[CLIENTE] Recebi ") << n << TEXT(" bytes: \'") << resp.msg << TEXT("\'... (ReadFile)\n"));
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
		_tcscpy(req.msg, TEXT("UP\n"));
		req.command = (int)MOVEUP;
		EscreveMensagem(pipe, req);
		//LerMensagem(pipe);
		break;
	}
	case KEY_DOWN:
	{
		//_fgetts(req.msg, 256, stdin);
		//memset(req.msg, '\0', sizeof(TCHAR));
		_tcscpy(req.msg, TEXT("Down\n"));
		req.command = (int)MOVEDOWN;
		EscreveMensagem(pipe, req);
		//LerMensagem(pipe);
		break;
	}
	case KEY_LEFT:
	{
		//_fgetts(req.msg, 256, stdin);
		memset(req.msg, '\0', sizeof(TCHAR));
		_tcscpy(req.msg, TEXT("Left\n"));
		req.command = (int)MOVELEFT;
		EscreveMensagem(pipe, req);
		//LerMensagem(pipe);
		break;
	}
	case KEY_RIGHT:
	{
		//_fgetts(req.msg, 256, stdin);
		memset(req.msg, '\0', sizeof(TCHAR));
		_tcscpy(req.msg, TEXT("Right\n"));
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

int _tmain(int argc, LPTSTR argv[]) {
	HANDLE hPipe,hPipeJogo;
	HANDLE hThread;
	ClientRequest req;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	//iniciado = FALSE;
	//Ligar ao pipe (PIPE_NAME)
	
	_tprintf(TEXT("[CLIENTE]Esperar pelo pipe %s(WaitNamedPipe)\n"), PIPE_NAME);
	//_tprintf(TEXT("[CLIENTE]Esperar pelo pipe (WaitNamedPipe)\n"));

	//tcout << TEXT("[CLIENTE]Esperar pelo pipe \'") << PIPE_NAME << TEXT("\'(WaitNamedPipe)\n");
	if (!WaitNamedPipe(PIPE_NAME, NMPWAIT_WAIT_FOREVER)) {
		_tprintf(TEXT("[ERRO] Ligar ao pipe \'%s\'... (WaitNamedPipe)\n"), PIPE_NAME);
		//_tprintf(TEXT("[ERRO] Ligar ao pipe ... (WaitNamedPipe)\n"));
		//tcout << TEXT("[ERRO] Ligar ao pipe \'") << PIPE_NAME << TEXT("\'... (WaitNamedPipe)\n");
		system("pause");
		exit(-1);
	}
	_tprintf(TEXT("[CLIENTE] Ligação ao servidor... (CreateFile)\n"));
	//tcout << TEXT("[CLIENTE] Ligação ao servidor... (CreateFile)\n");
	if ((hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == NULL) {
		//tcerr << TEXT("[ERRO] Ligar ao pipe \'") << PIPE_NAME << TEXT("\'... (CreateFile)\n");
		_tprintf(TEXT("[ERRO] Ligar ao pipe \'%s\'... (WaitNamedPipe)\n"), PIPE_NAME);
		system("pause");
		exit(-1);
	}
	_tprintf(TEXT("[CLIENTE] Ligação ao servidor PIPE BROADCAST... (CreateFile)\n"));
	//tcout << TEXT("[CLIENTE] Ligação ao servidor... (CreateFile)\n");
	if ((hPipeJogo = CreateFile(PIPE_NAME_JOGO, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == NULL) {
		_tprintf(TEXT("[ERRO] Ligar ao pipe \'%s\'... (WaitNamedPipe)\n"), PIPE_NAME_JOGO);
		//tcerr << TEXT("[ERRO] Ligar ao pipe \'") << PIPE_NAME_JOGO << TEXT("\'... (CreateFile)\n");
		system("pause");
		exit(-1);
	}

	_tprintf(TEXT("[CLIENTE]Liguei-me...\n"));
	//tcout << TEXT("[CLIENTE]Liguei-me...\n");
	
	
	//hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EsperaComando, (LPVOID)hPipe, 0, NULL);

	//WaitForSingleObject(hThread, INFINITE);

	int kp = 0;		//keypress
	int seta = 0;	//Posicionamento da seta no ecran
	MenuInicial(seta);
	while(!fim)
	{
		kp = Getch();
		switch (kp)
		{
		case KEY_UP:
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
		case KEY_DOWN:
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
		case KEY_ENTER:
			//caso 1: posiciona o jogador no server, pede LOS mostra HUD
			
			if (seta == 0)
			{
				
				iniciado = TRUE;
				req.command = STARTGAME;
				_tcscpy(req.msg, TEXT("Inicia..."));
				//envia o codigo de inicio
				EscreveMensagem(hPipe, req);
				//Lê o "iniciaste"
				LerMensagem(hPipe);
				system("pause");

				/*entre estas funções é preciso esperar pelo setup dos players*/
				//Broadcast da posiçao
				//hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LerBroadcast, (LPVOID)hPipeJogo, 0, NULL);
				
				//envia movimentos agora
				//HANDLE hMove = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Movimento, (LPVOID)hPipeJogo, 0, NULL);
				int k = 0;
				while (1)
				{
					//clrscr();
					do {
						k = Getch();
					} while (k < 1);

					enviaTecla(k,hPipe);
					LerMensagem(hPipe);
				}
				//system("pause");
			}
			else if (seta == 1)
			{
				memset(req.msg, '\0', sizeof(TCHAR) * BUFFERSIZE);
				_tprintf(TEXT("[CLIENTE] Nome: "));
				_fgetts(req.msg, BUFFERSIZE, stdin);
				req.command = (int)SETNAME;
				EscreveMensagem(hPipe, req);
				LerMensagem(hPipe);
			}
			else
				fim = TRUE;
				//CloseHandle(hPipe);
				break;
			break;
		default:
			break;
		}
	}
	//CloseHandle(hPipe);
	exit(0);
}

