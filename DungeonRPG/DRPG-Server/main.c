#include "Server.h"
#include "Common.h"
#include "Labirinto.h"

int totalConnections = 0;

Labirinto gLabirinto;
Jogador gClients[MAX_CLIENTS];

BOOL start = FALSE;
BOOL fim = FALSE;

HANDLE mutexLabirinto;
HANDLE ghGameInstanceEvent;

int _tmain(int argc, LPTSTR argv[]) {
	HANDLE hThreadL;
	HANDLE hThreadE;

#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	if ((mutexLabirinto = CreateMutex(NULL, FALSE, TEXT("Labirinto ocupado"))) == NULL){
		printf("Criação do Mutex falhou (%d)\n", GetLastError());
		return;
	}

	if ((ghGameInstanceEvent = CreateEvent(NULL,TRUE,FALSE,TEXT("Instante de Jogo"))) == NULL) {
		printf("Criação do Evento falhou (%d)\n", GetLastError());
		return;
	}
		
	//gLabirinto = NovoLabirinto();
	gLabirinto = LerLabirinto(TEXT("jogo.txt"));

	//Invocar a thread que inscreve novos clientes
	hThreadL = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecebeClientes, NULL, 0, NULL);
	hThreadE = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ActualizaClientes, NULL, 0, NULL);


	//fim = TRUE; //define quando a thread deve terminar

	//Esperar a thread RecebeClientes terminar e fecha o handle
	WaitForSingleObject(hThreadL, INFINITE);
	WaitForSingleObject(hThreadE, INFINITE);
	CloseHandle(hThreadL);
	CloseHandle(hThreadE);

	//Espera pelas threads dos clientes e desliga-as
	for (size_t i = 0; i < totalConnections; i++)
		WaitForSingleObject(gClients[i].hThread, INFINITE);
	DesligarThreadsDeCliente();

	CloseHandle(mutexLabirinto);
	CloseHandle(ghGameInstanceEvent);
	exit(0);
}

