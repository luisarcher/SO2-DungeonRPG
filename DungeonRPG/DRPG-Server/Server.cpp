#include "Server.h"

HANDLE pipeClients[MAX_CLIENTS];
BOOL fim = FALSE;
int totalConnections = 0;

//isto depois tem de ir para o .h
void DesligarNamedPipes();
void debug_ServerBroadcasting();

int _tmain(int argc, LPTSTR argv[]) {
	HANDLE hThread; //handle para a thread que regista/liga os clientes

#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif
	//Invocar a thread que inscreve novos clientes
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecebeClientes, NULL, 0, NULL);

	//debug_ServerBroadcasting();

	//fim = TRUE; //define quando a thread deve terminar

	//Esperar a thread RecebeClientes terminar e fecha o handle
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

	exit(0);
}

//SignUp and connect to clients
DWORD WINAPI RecebeClientes(LPVOID param) {
	DWORD n;
	while (!fim && totalConnections < MAX_CLIENTS) {

		_tprintf(TEXT("[SERVER] Vou passar à criação de uma cópia do pipe '%s' ... (CreateNamedPipe)\n"), PIPE_NAME);
		
		if ((pipeClients[totalConnections] = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_WAIT | PIPE_TYPE_MESSAGE
			| PIPE_READMODE_MESSAGE, MAX_CLIENTS, BUFFERSIZE * sizeof(TCHAR), BUFFERSIZE * sizeof(TCHAR),
			1000, NULL)) == INVALID_HANDLE_VALUE) {
			_tperror(TEXT("Erro ao criar named pipe!"));
			exit(-1);
		}

		//Espera a ligação de um cliente (ConnectNamedPipe é bloqueante)
		_tprintf(TEXT("[SERVER] Esperar ligação de um cliente... (ConnectNamedPipe)\n"));
		if (!ConnectNamedPipe(pipeClients[totalConnections], NULL)) {
			_tperror(TEXT("Erro na ligação ao leitor!"));
			exit(-1);
		}

		//Atende o cliente que se ligou
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtendeCliente, (LPVOID)totalConnections, 0, &n);
		totalConnections++;
	}
	DesligarNamedPipes();
	return 0;
}

DWORD WINAPI AtendeCliente(LPVOID param){
	//ide buscar o handle do cliente que está no array global
	HANDLE hPipeCliente = pipeClients[(int)param];

	TCHAR buf[256];
	DWORD n;
	BOOL ret;

	while (1) {
		ret = ReadFile(hPipeCliente, buf, sizeof(buf), &n, NULL);
		//buf[n / sizeof(TCHAR)] = '\0';
		if (!ret || !n)
			break;
		tcout << TEXT("[Server] Recebi ") << n << TEXT(" bytes: \'") << buf << TEXT("\'... (ReadFile)\n");

		/* Enviar para todos */
		_tcscpy(buf, TEXT("Os clientes são todos uns conas"));
		for (int i = 0; i < totalConnections; i++)
			if (!WriteFile(pipeClients[i], buf, _tcslen(buf)*sizeof(TCHAR), &n, NULL)) {
				_tperror(TEXT("[ERRO] Escrever no pipe... (WriteFile)\n"));
				exit(-1);
			}

	}
	return 0;
}

//Disconnect from all pipes
void DesligarNamedPipes() {
	for (int i = 0; i < totalConnections; i++) {
		DisconnectNamedPipe(pipeClients[i]);
		_tprintf(TEXT("[ESCRITOR] Vou desligar o pipe... (CloseHandle)\n"));
		CloseHandle(pipeClients[i]);
	}
}

void debug_ServerBroadcasting() {
	TCHAR buf[BUFFERSIZE];
	DWORD n;
	do {
		_tprintf(TEXT("[SERVER] Frase: "));
		_fgetts(buf, 256, stdin);
		//Escrever para todos os clientes inscritos
		for (int i = 0; i < totalConnections; i++)
			if (!WriteFile(pipeClients[i], buf, _tcslen(buf)*sizeof(TCHAR), &n, NULL)) {
				_tperror(TEXT("[ERRO] Escrever no pipe... (WriteFile)\n"));
				exit(-1);
			}

		_tprintf(TEXT("[SERVER] Enviei %d bytes aos %d clientes... (WriteFile)\n"), n, totalConnections);
	} while (_tcsncmp(buf, TEXT("fim"), 3));
}