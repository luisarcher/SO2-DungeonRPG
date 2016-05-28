#include "Client.h"
#include "UI.h"

int _tmain(int argc, LPTSTR argv[]) {
	HANDLE hPipe, hPipeJogo;
	HANDLE hThread;
	ClientRequest req;
	TCHAR buf[BUFFERSIZE];

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	InicializarPipes(&hPipe, &hPipeJogo);

	int kp = 0;		//keypress
	int seta = 0;	//Posicionamento da seta no ecran
	MenuInicial(seta);
	while (!fim)
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
				_tcscpy(req.msg, TEXT("START"));
				//envia o codigo de inicio
				EscreveMensagem(hPipe, req);
				//Lê o "iniciaste"
				LerMensagem(hPipe,buf);
				_tprintf(TEXT("[SERVIDOR] %s"), buf);

				/*entre estas funções é preciso esperar pelo setup dos players*/
				//Broadcast da posiçao
				hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LerBroadcast, (LPVOID)hPipeJogo, 0, NULL);

				//envia movimentos agora
				//HANDLE hMove = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Movimento, (LPVOID)hPipeJogo, 0, NULL);
				int k = 0;
				while (1)
				{
					//clrscr();
					do {
						k = Getch();
					} while (k < KEY_UP || k > KEY_RIGHT);

					enviaTecla(k, hPipe);
					LerMensagem(hPipe, buf);
					_tprintf(TEXT("[SERVIDOR] %s"), buf);

					//Ler mensagem de difusão
					if (iniciado == TRUE)
						MostraLOS(resp.matriz);
					if (resp.msg[0] != TEXT('\0')) {
						_tprintf(TEXT("[SERVIDOR] %s"), resp.msg);
					}
				}
			}
			else if (seta == 1)
			{
				memset(req.msg, '\0', sizeof(TCHAR) * BUFFERSIZE);
				_tprintf(TEXT("[CLIENTE] Nome: "));
				_fgetts(req.msg, BUFFERSIZE, stdin);
				req.command = (int)SETNAME;
				EscreveMensagem(hPipe, req);
				LerMensagem(hPipe, buf);
				_tprintf(TEXT("[SERVIDOR] %s"), buf);
			}
			else
				fim = TRUE;
			break;
		default:
			break;
		}
	}
	FecharHandles(&hPipe, &hPipeJogo);
	exit(0);
}