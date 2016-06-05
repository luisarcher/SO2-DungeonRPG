#include "Monster.h"
#include "Common.h"

Labirinto * shLabirinto;
BOOL fim = FALSE;

int _tmain(int argc, LPTSTR argv[]) {
	HANDLE hMappedObj;
	HANDLE hGameInstanceEvent;

	Monstro m = NovoMonstroBully();
#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	hGameInstanceEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, TEXT("gameInstanceEvent"));
	InitializeSharedMemory(&hMappedObj);
	
	_tprintf(TEXT("Vou ler o estado do labirinto...\n"));
	system("pause");
	m.posX = 1;
	m.posY = 2;
	escondeCursor();
	shLabirinto->labirinto[m.posY][m.posX] = m.tipo;
	
	while (!fim) { 
		//Espera pelo evento ser sinalizado do lado do servidor
		WaitForSingleObject(hGameInstanceEvent, INFINITE);
		//... Passou 1 instante... segue...

		ReadSharedMemory(); 
		

		MoveMonstro(shLabirinto, m.direcao, &m);
	}

	CloseHandles(&hMappedObj);
}