#include "Client.h"

int _tmain(int argc, LPTSTR argv[]) {
	TCHAR buf[256];
	HANDLE hPipe;
	int i = 0;
	BOOL ret;
	DWORD n;
	HANDLE hThread;
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

	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EscrevePipe, (LPVOID)hPipe, 0, NULL);

	tcout << TEXT("[CLIENTE]Liguei-me...\n");
	/*while (1) {

	}*/ WaitForSingleObject(hThread, INFINITE);
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