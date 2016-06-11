#include "SetupGame.h"
#include "GameData.h"

HDC		bitmapElementsDC[N_BITMAPS];
HBITMAP	bitmapElementsOrigin[N_BITMAPS];

ATOM RegistaClasse(HINSTANCE hInstance, TCHAR * szWinName) {
	WNDCLASSEX wcApp;	// WNDCLASSEX é uma estrutura cujos membros servem para definir as características da classe da janela
	
	/* Definição das características da janela */
	wcApp.cbSize = sizeof(WNDCLASSEX);	// Tamanho da estrutura WNDCLASSEX
	wcApp.hInstance = hInstance;			// Instância da janela actualmente exibida ("hInst" é parâmetro de WinMain e vem inicializada daí).
	wcApp.lpszClassName = szWinName;	// Nome da janela (neste caso = nome do programa)
	wcApp.lpfnWndProc = WinProc;		// Endereço da função de processamento da janela
	wcApp.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;			// Estilo da janela: Fazer o redraw vertical, horizontar, e poder capturar duplo Cliques.
	wcApp.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// "hIcon" = handler do ícon normal | "NULL" = Icon definido no Windows | "IDI_AP..." Ícone "aplicação"
	wcApp.hIconSm = LoadIcon(NULL, IDI_INFORMATION);			// "hIcon" = handler do ícon pequeno | Ver documentação prof.
	wcApp.hCursor = LoadCursor(NULL, IDC_ARROW);				// "hCursor" = handler do cursor (rato) | Ver documentação prof.
	wcApp.lpszMenuName = IDR_MENU1;							// Classe do menu que a janela pode ter | (NULL = não tem menu)
	wcApp.cbClsExtra = 0;							// Livre, para uso particular
	wcApp.cbWndExtra = 0;							// Livre, para uso particular
	wcApp.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	/*OUTRAS CORES DE BRUSH:	BLACK_BRUSH  DKGRAY_BRUSH GRAY_BRUSH LTGRAY_BRUSH  */

	// ============================================================================
	// 2. Registar a classe "wcApp" no Windows
	// ============================================================================
	return(RegisterClassEx(&wcApp));
}

HWND CriarJanela(HINSTANCE hInstance, TCHAR * szWinName) {
	// ============================================================================
	// 3. Criar a janela
	// ============================================================================
	return CreateWindow(
		szWinName,
		TEXT("Dungeon RPG - Sistemas Operativos 2"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(HWND)HWND_DESKTOP,
		(HMENU)NULL,
		(HINSTANCE)hInstance,
		0);
}

int CarregarTodasAsImagens() {
	for (int i = 0; i < N_BITMAPS; i++) {
		bitmapElements[i] = LoadBitmap(GetModuleHandle(NULL),_bitmaps[i]);
		if (bitmapElements[i] == NULL) {
			return i*(-1);
		}
	}
	return 1;
}

void ConfigurarDCs(HDC hdcOrigin) {
	for (int i = 0; i < N_BITMAPS; i++)
	{
		bitmapElementsDC[i] = CreateCompatibleDC(hdcOrigin);
		bitmapElementsOrigin[i] = SelectObject(bitmapElementsDC[i],bitmapElements[i]);
	}
}

DWORD WINAPI AtualizaJogo(LPVOID lpParam) {
	DWORD nBytes;
	HWND hWnd = (HWND)lpParam;
	while (!fim) {
		if (pendingChangesFlag) {
			pendingChangesFlag = 0;
			//Analisa a estrutura recebida em resp
			//if(resp.playerInfo.hp > 0)
				InvalidateRect(hWnd, NULL, FALSE);
		}
	}
	return 0;
}