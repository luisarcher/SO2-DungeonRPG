#include "Controller.h"
#include "lib\ClientDLL.h"

int EnviaTecla(int keystroke) {
	return 0;
}

TCHAR * Registar(TCHAR * nome) {
	TCHAR resposta[BUFFERSIZE];
	ClientRequest cr;
	cr.command = SETNAME;
	_tcscpy(cr.msg, nome);
	if (!EscreveMensagem(hPipe, cr)) return 0;
	else LerMensagem(hPipe,resposta);
	return resposta;
}