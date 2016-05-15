#include "Server.h"
#include "Common.h"
#include "Labirinto.h"

Labirinto gLabirinto; //Isto vai para memória partilhada com os monstros (META2)

int _tmain(int argc, LPTSTR argv[]) {
	HANDLE hThreadL;
	HANDLE hThreadE;

#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	gLabirinto = NovoLabirinto();

	//Invocar a thread que inscreve novos clientes
	hThreadL = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecebeClientes, NULL, 0, NULL);
	hThreadE = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ActualizaClientes, NULL, 0, NULL);
	//fim = TRUE; //define quando a thread deve terminar

	//Esperar a thread RecebeClientes terminar e fecha o handle
	WaitForSingleObject(hThreadL, INFINITE);
	CloseHandle(hThreadL);

	exit(0);
}

