#include "ClientDLL.h"

ServerResponse resp;
BOOL fim = FALSE;

/**
*	Liga ao servidor.
*	Retorna 1,
*	caso contrário, código associado ao erro.
*/
int InicializarPipes(HANDLE * hPipe, HANDLE * hPipeJogo, TCHAR * ipServ) {
	TCHAR pipeName[BUFFERSIZE] = TEXT("\\\\");
	TCHAR pipeNameJogo[BUFFERSIZE] = TEXT("\\\\");
	if (ipServ == NULL) {
		_tcscat(pipeName, TEXT("."));
		_tcscpy(pipeName, TEXT("."));
	}
	else {
		_tcscat(pipeName, ipServ);
		_tcscat(pipeNameJogo, ipServ);
	}
	_tcscat(pipeName, PIPE_NAME_SUFFIX);
	_tcscat(pipeNameJogo, PIPE_NAME_JOGO_SUFFIX);

	if (!WaitNamedPipe(pipeName, NMPWAIT_WAIT_FOREVER)) {
		return 0;
	}
	if ((*hPipe = CreateFile(pipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == NULL) {
		return -1;
	}
	if ((*hPipeJogo = CreateFile(pipeNameJogo, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == NULL) {
		return -2;
	}
	return 1;
}

/**
*	Envia uma mensagem ao servidor.
*	Retorna o número de bytes enviados.
*/
DWORD EscreveMensagem(HANDLE pipe, ClientRequest req) {
	DWORD n = 0;
	WriteFile(pipe, &req, sizeof(ClientRequest), &n, NULL);
	return n;
}

/**
*	Lê uma mensagem do servidor.
*	Retorna o número de bytes recebidos.
*/
DWORD LerMensagem(HANDLE pipe, TCHAR * serverResponse) {
	DWORD n = 0;
	ReadFile(pipe, serverResponse, sizeof(TCHAR) * BUFFERSIZE, &n, NULL);
	return n;
}

/**
*	Recebe as mensagens de difusão do servidor,
*	para a var. <resp> a exportar.
*	Retorna o último número de bytes recebidos.
*/
DWORD WINAPI LerBroadcast(LPVOID param) {
	HANDLE pipe = (HANDLE)param;
	DWORD n = 0;

	while (!fim)
		ReadFile(pipe, &resp, sizeof(ServerResponse), &n, NULL);
	return n;
}

/**
*	Fecha os handles necessários á execução do servidor.
*/
void FecharHandles(HANDLE * hPipe, HANDLE * hPipeJogo) {
	CloseHandle(*hPipe);
	CloseHandle(*hPipeJogo);
}

/*BOOL WINAPI DllMain(HANDLE hInstance, DWORD razao, LPVOID tipo_declaracao) {
	switch (razao) {
	case DLL_PROCESS_ATTACH:
		if (tipo_declaracao == NULL)
			MessageBox(NULL, TEXT("O processou carregou a biblioteca explicitamente"), TEXT("DLLmain"), MB_OK);
		else
			MessageBox(NULL, TEXT("O processou carregou a biblioteca implicitamente"), TEXT("DLLmain"), MB_OK);
		break;
	case DLL_PROCESS_DETACH:
		if (tipo_declaracao == NULL)
			MessageBox(NULL, TEXT("O processou desmapeou a biblioteca explicitamente"), TEXT("DLLmain"), MB_OK);
		else
			MessageBox(NULL, TEXT("O processou desmapeou a biblioteca implicitamente"), TEXT("DLLmain"), MB_OK);
		break;
	}
	return 1;
}*/


