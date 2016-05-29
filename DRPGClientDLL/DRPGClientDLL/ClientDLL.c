#include "ClientDLL.h"

ServerResponse resp;
BOOL fim = FALSE;

void InicializarPipes(HANDLE * hPipe, HANDLE * hPipeJogo) {
	_tprintf(TEXT("[CLIENTE]Esperar pelo pipe %s(WaitNamedPipe)\n"), PIPE_NAME);
	if (!WaitNamedPipe(PIPE_NAME, NMPWAIT_WAIT_FOREVER)) {
		_tprintf(TEXT("[ERRO] Ligar ao pipe \'%s\'... (WaitNamedPipe)\n"), PIPE_NAME);
		system("pause");
		exit(-1);
	}
	_tprintf(TEXT("[CLIENTE] Ligação ao servidor... (CreateFile)\n"));
	if ((*hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == NULL) {
		_tprintf(TEXT("[ERRO] Ligar ao pipe \'%s\'... (WaitNamedPipe)\n"), PIPE_NAME);
		system("pause");
		exit(-1);
	}
	_tprintf(TEXT("[CLIENTE] Ligação ao servidor PIPE BROADCAST... (CreateFile)\n"));
	if ((*hPipeJogo = CreateFile(PIPE_NAME_JOGO, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == NULL) {
		_tprintf(TEXT("[ERRO] Ligar ao pipe \'%s\'... (WaitNamedPipe)\n"), PIPE_NAME_JOGO);
		system("pause");
		exit(-1);
	}
	_tprintf(TEXT("[CLIENTE]Ligação Estabelecida!\n"));
}

void EscreveMensagem(HANDLE pipe, ClientRequest req) {
	DWORD n;
	if (!WriteFile(pipe, &req, sizeof(ClientRequest), &n, NULL)) {
		_tprintf(TEXT("[CLIENTE] Erro ao enviar mensagem ao servidor!\n"));
		system("pause");
		exit(-1);
	}
	//else _tprintf(TEXT("[CLIENTE]Pedido enviado [%d-%s]\n"),req.command,req.msg); 
}

void LerMensagem(HANDLE pipe, TCHAR * serverResponse) {
	DWORD n;
	if (!ReadFile(pipe, serverResponse, sizeof(TCHAR) * BUFFERSIZE, &n, NULL)) {
		_tperror(TEXT("[CLIENTE] Erro ao ler mensagem do servidor!\n"));
		system("pause");
		exit(-1);
	}
}

DWORD WINAPI LerBroadcast(LPVOID param) {
	HANDLE pipe = (HANDLE)param;
	DWORD n;

	while (!fim)
		if (!ReadFile(pipe, &resp, sizeof(ServerResponse), &n, NULL)) {
			_tperror(TEXT("[CLIENTE] Erro ao ler dados de difusão!\n"));
			system("pause");
			exit(-1);
		}
	return 0;
}

void FecharHandles(HANDLE * hPipe, HANDLE * hPipeJogo) {
	CloseHandle(hPipe);
	CloseHandle(hPipeJogo);
}

BOOL WINAPI DllMain(HANDLE hInstance, DWORD razao, LPVOID tipo_declaracao) {
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
}


