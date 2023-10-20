#include "Controller.h"
#include "./../../DRPGClientDLL/DRPGClientDLL/ClientDLL.h"

DWORD SendCommand(int _command) {
	ClientRequest cr;

	cr.command = _command;
	_tcscpy(cr.msg, TEXT("\0"));

	if (!EscreveMensagem(hPipe, cr)) return 0;
	return LerMensagem(hPipe, receivedMSG);
}

DWORD Register(TCHAR * nome) {
	ClientRequest cr;

	cr.command = SETNAME;
	_tcscpy(cr.msg, nome);

	if (!EscreveMensagem(hPipe, cr)) return 0;
	return LerMensagem(hPipe, receivedMSG);
}

DWORD CallStartGame() {
	ClientRequest cr;

	cr.command = STARTGAME;
	_tcscpy(cr.msg, TEXT("\0"));

	if (!EscreveMensagem(hPipe, cr)) return 0;
	return LerMensagem(hPipe, receivedMSG);
}