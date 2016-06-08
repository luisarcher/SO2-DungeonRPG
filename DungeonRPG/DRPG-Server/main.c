#include "Server.h"
#include "Common.h"
#include "Labirinto.h"

int totalConnections = 0;

//Labirinto gLabirinto;
Labirinto * shLabirinto;
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
	HANDLE hThreadGameEvents;
	HANDLE hFileToMap, hMappedObj;

#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	if ((gMutexLabirinto = CreateMutex(NULL, FALSE, TEXT("LabirintoOcupado"))) == NULL){
		printf("Criação do Mutex falhou (%d)\n", GetLastError());
		return;
	}

	//Manual reset event
	if ((ghGameInstanceEvent = CreateEvent(NULL,TRUE,FALSE,TEXT("gameInstanceEvent"))) == NULL) {
		printf("Criação do Evento falhou (%d)\n", GetLastError());
		return;
	}

	//Auto-reset Event
	if ((ghUpdateGameClientEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("ActualizarClientes"))) == NULL) {
		printf("Criação do Evento falhou (%d)\n", GetLastError());
		return;
	}
	/*****  MEMÓRIA PARTILHADA  *****/
	hFileToMap = CreateFile(TEXT("DungeonRPG"),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFileToMap == INVALID_HANDLE_VALUE) {
		_tprintf(TEXT("[Erro] Abrir ficheiros (%d)\n"), GetLastError());
		return -1;
	}

	hMappedObj = CreateFileMapping(hFileToMap,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(Labirinto),
		TEXT("ShLabirinto")
	);
	if (hMappedObj == NULL) {
		_tprintf(TEXT("[Erro] Criar objectos mapeamentos(%d)\n"), GetLastError());
		return -1;
	}

	shLabirinto = (Labirinto*)MapViewOfFile(hMappedObj,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(Labirinto)
	);
	if (shLabirinto == NULL) {
		_tprintf(TEXT("[Erro] Mapear para memória(%d)\n"), GetLastError());
		return -1;
	}
	/*****  MEMÓRIA PARTILHADA ENDS *****/
		
	//gLabirinto = NovoLabirinto();
	*shLabirinto = LerLabirinto();
	DistribuirItems();

	/* ####### LANÇAR MONSTROS ########## */
	/*int nMonsters = ((rand() % 5) + 5); // 5~9
	for (size_t i = 0; i <  nMonsters; i++)
	{

	}*/
	/* ################################## */

	hThreadListener = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecebeClientes, NULL, 0, NULL);
	hThreadSender	= CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ActualizaClientes, NULL, 0, NULL);
	hThreadGameTime		= CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GameTimer, NULL, 0, NULL);
	hThreadGameEvents	= CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GameEvents, NULL, 0, NULL);

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
	CloseHandle(ghUpdateGameClientEvent);

	UnmapViewOfFile(shLabirinto);
	CloseHandle(hMappedObj);
	CloseHandle(hFileToMap);
	exit(0);
}

