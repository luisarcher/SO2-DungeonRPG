#include "Server.h"
#include "Common.h"
#include "Labirinto.h"

Labirinto l;
Jogador j;


int _tmain(int argc, LPTSTR argv[]) {
	HANDLE hThread;

#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif
	//Invocar a thread que inscreve novos clientes
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecebeClientes, NULL, 0, NULL);
	//fim = TRUE; //define quando a thread deve terminar

	//if(fim == TRUE) 
	//Esperar a thread RecebeClientes terminar e fecha o handle
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);


	exit(0);
}

