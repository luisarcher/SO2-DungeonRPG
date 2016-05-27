#include "Monster.h"
#include "Common.h"

Labirinto * shLabirinto;
BOOL fim = FALSE;

int _tmain(int argc, LPTSTR argv[]) {
	HANDLE hMappedObj;

#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	InitializeSharedMemory(&hMappedObj);
	
	_tprintf(TEXT("Vou ler o estado do labirinto...\n"));
	system("pause");
	ReadSharedMemory();

	CloseHandles(&hMappedObj);
}