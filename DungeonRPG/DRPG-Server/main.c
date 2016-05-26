#include "Server.h"
#include "Common.h"
#include "Labirinto.h"

int totalConnections = 0;

Labirinto gLabirinto;
Jogador gClients[MAX_CLIENTS];

BOOL start = FALSE;
BOOL fim = FALSE;

HANDLE gMutexLabirinto;
HANDLE ghGameInstanceEvent;
HANDLE ghUpdateGameClientEvent;

int _tmain(int argc, LPTSTR argv[]) {
	HANDLE hThreadListener;
	HANDLE hThreadSender;
	HANDLE hThreadGameTime;

#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	if ((gMutexLabirinto = CreateMutex(NULL, FALSE, TEXT("Labirinto ocupado"))) == NULL){
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
	hThreadListener = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecebeClientes, NULL, 0, NULL);
	hThreadSender = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ActualizaClientes, NULL, 0, NULL);
	hThreadGameTime = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GameTimer, NULL, 0, NULL);
	HANDLE PLS = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GameEvents, NULL, 0, NULL);


	//fim = TRUE; //define quando as threads devem terminar

	//Esperar a thread RecebeClientes terminar e fecha o handle
	WaitForSingleObject(hThreadListener, INFINITE);
	WaitForSingleObject(hThreadSender, INFINITE);
	CloseHandle(hThreadListener);
	CloseHandle(hThreadSender);

	//Espera pelas threads dos clientes e desliga-as
	for (int i = 0; i < totalConnections; i++)
		WaitForSingleObject(gClients[i].hThread, INFINITE);
	DesligarThreadsDeCliente();

	CloseHandle(gMutexLabirinto);
	CloseHandle(ghGameInstanceEvent);
	exit(0);
}

