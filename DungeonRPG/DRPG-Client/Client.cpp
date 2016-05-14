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
typedef struct CLIENTREQUEST {
	int command;
	TCHAR msg[BUFFERSIZE];
} ClientRequest;

void EscreveMensagem(HANDLE pipe, ClientRequest req) {
	DWORD n;
	WriteFile(pipe, &req, sizeof(req), &n, NULL);
	tcout << TEXT("[CLIENTE]Escrevi...\n");
}

void LerMensagem() {

}


int _tmain(int argc, LPTSTR argv[]) {
	TCHAR buf[256];
	HANDLE hPipe;
	//int i = 0;
	BOOL iniciado = FALSE;
	BOOL ret;
	DWORD n;
	HANDLE hThread;
	ClientRequest req;
#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif

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

	//hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EscrevePipe, (LPVOID)hPipe, 0, NULL);

	tcout << TEXT("[CLIENTE]Liguei-me...\n");
	iniciado = TRUE;
	req.command = 1;
	
	while (iniciado) {
		MenuInicial(1);
		EscreveMensagem(hPipe, req);
		//tcout << TEXT("[CLIENTE]Liguei-me...\n");
		break;
	} 
	//WaitForSingleObject(hThread, INFINITE);
	
	CloseHandle(hPipe);

	Sleep(200);
	system("pause");
	return 0;
}

DWORD WINAPI EscrevePipe(LPVOID param) {
	HANDLE pipe = (HANDLE)param;
	TCHAR buf[256];
	DWORD n;
	BOOL ret;

	while (1) {
		//ler do terminal
		_tprintf(TEXT("[CLIENTE] Frase: "));
		_fgetts(buf, 256, stdin);

		WriteFile(pipe, buf, 256, &n, NULL);

		/* Ler resposta */
		ret = ReadFile(pipe, buf, sizeof(buf), &n, NULL);
		buf[n / sizeof(TCHAR)] = '\0';
		if (!ret || !n)
			break;
		tcout << TEXT("[CLIENTE] Recebi ") << n << TEXT(" bytes: \'") << buf << TEXT("\'... (ReadFile)\n");
	}
}

