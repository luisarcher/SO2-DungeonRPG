#include "Common.h"
#include "GameProc.h"

HINSTANCE ghInstance;
HBITMAP bitmapElements[N_BITMAPS];

HANDLE hPipe;		//Pipe para tratar pedidos
HANDLE hPipeJogo;	//Pipe para enviar estado do mapa e do jogador
HANDLE hThreadBroadcastReceiver;

TCHAR receivedMSG[BUFFERSIZE];
ServerResponse resp;
int connected = 0;
int started = 0;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {

	HWND hWnd;			// hWnd � o handler da janela, gerado mais abaixo por CreateWindow()
	MSG lpMsg;			// MSG � uma estrutura definida no Windows para as mensagens

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
	UpdateWindow(hWnd);				// Refrescar a janela (Windows envia � janela uma mensagem para pintar, mostrar dados, (refrescar), etc)

	while (GetMessage(&lpMsg, NULL, 0, 0)) {
		TranslateMessage(&lpMsg);			// Pr�-processamento da mensagem, p.e. obter c�digo ASCII da tecla premida
		DispatchMessage(&lpMsg);			// Enviar a mensagem traduzida de volta ao Windows, que aguarda at� que a possa enviar ao tratamento da janela.
	}

	WaitForSingleObject(hThreadBroadcastReceiver,INFINITE);
	void UnlinkObjects();
	FecharHandles(&hPipe, &hPipeJogo);

	return((int)lpMsg.wParam);
}


