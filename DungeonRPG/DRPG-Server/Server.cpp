#include "Server.h"

HANDLE PipeClients[MAX_CLIENTS];
BOOL fim = FALSE;
int totalConnections = 0;

int _tmain(int argc, LPTSTR argv[]) {
	DWORD n;
	HANDLE hThread;
	TCHAR buf[BUFFERSIZE];

#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif
	//Invocar a thread que inscreve novos clientes
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecebeClientes, NULL, 0, NULL);

	do {
		_tprintf(TEXT("[SERVER] Frase: "));
		_fgetts(buf, 256, stdin);
		//Escrever para todos os clientes inscritos
		for (int i = 0; i < totalConnections; i++)
			if (!WriteFile(PipeClients[i], buf, _tcslen(buf)*sizeof(TCHAR), &n, NULL)) {
				_tperror(TEXT("[ERRO] Escrever no pipe... (WriteFile)\n"));
				exit(-1);
			}

		_tprintf(TEXT("[SERVER] Enviei %d bytes aos %d clientes... (WriteFile)\n"), n, totalConnections);
	} while (_tcsncmp(buf, TEXT("fim"), 3));
	fim = TRUE;

	//Esperar a thread RecebeClientes terminar
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	exit(0);
}

DWORD WINAPI RecebeClientes(LPVOID param) {
	while (!fim && totalConnections < MAX_CLIENTS) {

		_tprintf(TEXT("[SERVER] Vou passar à criação de uma cópia do pipe '%s' ... (CreateNamedPipe)\n"), PIPE_NAME);
		PipeClients[totalConnections] = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_WAIT | PIPE_TYPE_MESSAGE
			| PIPE_READMODE_MESSAGE, MAX_CLIENTS, BUFFERSIZE * sizeof(TCHAR), BUFFERSIZE * sizeof(TCHAR),
			1000, NULL);

		if (PipeClients[totalConnections] == INVALID_HANDLE_VALUE) {
			_tperror(TEXT("Erro na ligação ao leitor!"));
			exit(-1);
		}
		_tprintf(TEXT("[SERVER] Esperar ligação de um cliente... (ConnectNamedPipe)\n"));
		if (!ConnectNamedPipe(PipeClients[totalConnections], NULL)) {
			_tperror(TEXT("Erro na ligação ao leitor!"));
			exit(-1);
		}
		totalConnections++;
	}
	for (int i = 0; i < totalConnections; i++) {
		DisconnectNamedPipe(PipeClients[i]);
		_tprintf(TEXT("[ESCRITOR] Vou desligar o pipe... (CloseHandle)\n"));
		CloseHandle(PipeClients[i]);
	}
	return 0;
}