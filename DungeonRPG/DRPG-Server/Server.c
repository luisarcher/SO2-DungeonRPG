#include "Server.h"

//SignUp and connect clients
DWORD WINAPI RecebeClientes(LPVOID param) {
	DWORD n;
	while (!fim && totalConnections < MAX_CLIENTS) {

		_tprintf(TEXT("[SERVER] Vou passar à criação de uma cópia do pipe '%s' ... \n"), PIPE_NAME);
		if ((pipeClients[totalConnections] = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_WAIT | PIPE_TYPE_MESSAGE
			| PIPE_READMODE_MESSAGE, MAX_CLIENTS, BUFFERSIZE * sizeof(TCHAR), BUFFERSIZE * sizeof(TCHAR),
			1000, NULL)) == INVALID_HANDLE_VALUE){
			_tperror(TEXT("Erro ao criar named pipe!"));
			exit(-1);
		}

		//Espera a ligação de um cliente (ConnectNamedPipe é bloqueante)
		_tprintf(TEXT("[SERVER] Esperar ligação de um cliente...\n"));
		if (!ConnectNamedPipe(pipeClients[totalConnections], NULL)) {
			_tperror(TEXT("Erro na ligação!"));
			exit(-1);
		}

		//Atende o cliente que se ligou
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtendeCliente, (LPVOID)totalConnections, 0, &n);
		totalConnections++;
	}
	DesligarNamedPipes(); //depois de fim //VERIFICAR!!
	return 0;
}

DWORD WINAPI AtendeCliente(LPVOID param) {
	//ide buscar o handle do cliente que está no array global
	HANDLE hPipeCliente = pipeClients[(int)param];

	TCHAR buf[256];
	DWORD n;
	BOOL ret;

	while (1) {
		ret = ReadFile(hPipeCliente, buf, sizeof(buf), &n, NULL);
		buf[n / sizeof(TCHAR)] = '\0';
		if (!ret || !n)
			break;
		_tprintf(TEXT("[Server] Recebi %d bytes: \'%s\'... (ReadFile)\n"),n,buf);

		/* Enviar para todos */
		ServerBroadcasting();

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

void ServerBroadcasting() { //Depois passa-se a estrutura por parâmetro
	TCHAR buf[BUFFERSIZE];
	DWORD n;
	//do {
		_tcscpy(buf, TEXT("Isto é o jogo vindo do servidor"));
		//Escrever para todos os clientes inscritos
		for (int i = 0; i < totalConnections; i++)
			if (!WriteFile(pipeClients[i], buf, _tcslen(buf)*sizeof(TCHAR), &n, NULL)) {
				_tperror(TEXT("[ERRO] Escrever no pipe... (WriteFile)\n"));
				exit(-1);
			}

		/*_tprintf(TEXT("[SERVER] Enviei %d bytes aos %d clientes... (WriteFile)\n"), n, totalConnections);
	} while (_tcsncmp(buf, TEXT("fim"), 3));*/
}