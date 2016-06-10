#include "Common.h"
#include "Proc.h"
#include "Init.h"

HINSTANCE ghInstance;

HANDLE hPipe;		//Pipe para tratar pedidos
HANDLE hPipeJogo;	//Pipe para enviar estado do mapa e do jogador

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {

	HWND hWnd;			// hWnd é o handler da janela, gerado mais abaixo por CreateWindow()
	MSG lpMsg;			// MSG é uma estrutura definida no Windows para as mensagens

	TCHAR *szProgName = TEXT("DungeonRPG");
	ghInstance = hInst;

	//Registar a classe no windows
	if (!RegistaClasse(hInst, szProgName))
		return 0;

	//Criar a janela
	if ((hWnd = CriarJanela(hInst, szProgName)) == NULL)
		return 0;

	//Mostrar Janela
	ShowWindow(hWnd, nCmdShow);		// "hWnd"= handler da janela, devolvido por "CreateWindow";
	UpdateWindow(hWnd);				// Refrescar a janela (Windows envia à janela uma mensagem para pintar, mostrar dados, (refrescar), etc)

	while (GetMessage(&lpMsg, NULL, 0, 0)) {
		TranslateMessage(&lpMsg);			// Pré-processamento da mensagem, p.e. obter código ASCII da tecla premida
		DispatchMessage(&lpMsg);			// Enviar a mensagem traduzida de volta ao Windows, que aguarda até que a possa enviar ao tratamento da janela.
	}
	return((int)lpMsg.wParam);
}


