#include "Server.h"
#include "Common.h"
#include "GameBoard.h"

int totalConnections = 0;

//GameBoard gGameBoard;
GameBoard* shGameBoard;
Player gClients[MAX_CLIENTS];

BOOL start = FALSE;
BOOL fim = FALSE;

HANDLE gMutexGameBoard;
HANDLE ghGameInstanceEvent;
HANDLE ghUpdateGameClientEvent;

int _tmain(int argc, LPTSTR argv[]) {
	HANDLE hThreadListener;
	HANDLE hThreadSender;
	HANDLE hThreadGameTime;
	HANDLE hThreadGameEvents;
	HANDLE hFileToMap, hMappedObj;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	
	
#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	if ((gMutexGameBoard = CreateMutex(NULL, FALSE, TEXT("GameBoardOcupado"))) == NULL){
		_tprintf(TEXT("Criação do Mutex falhou (%d)\n"), GetLastError());
		return;
	}

	//Manual reset event
	if ((ghGameInstanceEvent = CreateEvent(NULL,TRUE,FALSE,TEXT("gameInstanceEvent"))) == NULL) {
		_tprintf(TEXT("Criação do Evento falhou (%d)\n"), GetLastError());
		return;
	}

	//Auto-reset Event
	if ((ghUpdateGameClientEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("ActualizarClientes"))) == NULL) {
		_tprintf(TEXT("Criação do Evento falhou (%d)\n"), GetLastError());
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
		sizeof(GameBoard),
		TEXT("ShGameBoard")
	);
	if (hMappedObj == NULL) {
		_tprintf(TEXT("[Erro] Criar objectos mapeamentos(%d)\n"), GetLastError());
		return -1;
	}

	shGameBoard = (GameBoard*)MapViewOfFile(hMappedObj,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(GameBoard)
	);
	if (shGameBoard == NULL) {
		_tprintf(TEXT("[Erro] Mapear para memória(%d)\n"), GetLastError());
		return -1;
	}
	/*****  MEMÓRIA PARTILHADA ENDS *****/
		
	//gGameBoard = NovoGameBoard();
	*shGameBoard = LerGameBoard();
	DistribuirItems();

	/* ####### LANÇAR MONSTROS ########## */
	int nMonsters = ((rand() % MONSTERS_MAX + 1) + MONSTERS_MIN);
	TCHAR path[512];
	
	
	//for (size_t i = 0; i < nMonsters; i++)
	for (size_t i = 0; i < 2; i++)
	{
		int x = 0;
		int y = 0;
		do {
			//srand(time(NULL));
			x = (rand() % GAMEBOARDSIZE);
			y = (rand() % GAMEBOARDSIZE);
		} while (!(shGameBoard->gameBoard[y][x] == EMPTY));

		
		_stprintf_s(path, 256, TEXT("\DRPG-MonsterAI.exe %d %d %d %d %d"), 51, 4, 10,x, y);
		ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		CreateProcess(
			NULL, 
			path, 
			NULL, 
			NULL, 
			0, 
			CREATE_NEW_CONSOLE,
			NULL, 
			NULL, 
			&si, 
			&pi);

	}

	/* ################################## */

	hThreadListener = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ListenForConnections, NULL, 0, NULL);
	hThreadSender	= CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ActualizaClientes, NULL, 0, NULL);
	hThreadGameTime		= CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GameTimer, NULL, 0, NULL);
	hThreadGameEvents	= CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GameEvents, NULL, 0, NULL);

	//fim = TRUE; //define quando as threads devem terminar

	//Esperar e fechar threads
	WaitForSingleObject(hThreadListener,	INFINITE);
	WaitForSingleObject(hThreadSender,		INFINITE);
	WaitForSingleObject(hThreadGameTime,	INFINITE);
	WaitForSingleObject(hThreadGameEvents,	INFINITE);
	CloseHandle(hThreadListener);
	CloseHandle(hThreadSender);
	CloseHandle(hThreadGameTime);
	CloseHandle(hThreadGameEvents);

	//Espera pelas threads dos clientes e desliga-as
	for (int i = 0; i < totalConnections; i++)
		WaitForSingleObject(gClients[i].hThread, INFINITE);
	DesligarThreadsDeCliente();

	CloseHandle(gMutexGameBoard);
	CloseHandle(ghGameInstanceEvent);
	CloseHandle(ghUpdateGameClientEvent);

	UnmapViewOfFile(shGameBoard);
	CloseHandle(hMappedObj);
	CloseHandle(hFileToMap);
	exit(0);
}

