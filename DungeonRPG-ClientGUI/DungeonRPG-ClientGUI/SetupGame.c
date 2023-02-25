#include "SetupGame.h"
#include "GameData.h"

HDC		bitmapElementsDC[N_BITMAPS];
HBITMAP	bitmapElementsOrigin[N_BITMAPS];

ATOM RegistaClasse(HINSTANCE hInstance, TCHAR * szWinName) {
	WNDCLASSEX wcApp;	// WNDCLASSEX � uma estrutura cujos membros servem para definir as caracter�sticas da classe da janela
	
	/* Defini��o das caracter�sticas da janela */
	wcApp.cbSize = sizeof(WNDCLASSEX);	// Tamanho da estrutura WNDCLASSEX
	wcApp.hInstance = hInstance;			// Inst�ncia da janela actualmente exibida ("hInst" � par�metro de WinMain e vem inicializada da�).
	wcApp.lpszClassName = szWinName;	// Nome da janela (neste caso = nome do programa)
	wcApp.lpfnWndProc = WinProc;		// Endere�o da fun��o de processamento da janela
	wcApp.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;			// Estilo da janela: Fazer o redraw vertical, horizontar, e poder capturar duplo Cliques.
	wcApp.hIcon = LoadIcon(hInstance, IDI_ICON1);				// "hIcon" = handler do �con normal | "NULL" = Icon definido no Windows | "IDI_AP..." �cone "aplica��o"
	wcApp.hIconSm = LoadIcon(hInstance, IDI_ICON1);			// "hIcon" = handler do �con pequeno | Ver documenta��o prof.
	wcApp.hCursor = LoadCursor(NULL, IDC_ARROW);				// "hCursor" = handler do cursor (rato) | Ver documenta��o prof.
	wcApp.lpszMenuName = IDR_MENU1;							// Classe do menu que a janela pode ter | (NULL = n�o tem menu)
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

void UnlinkObjects() {
	for (int i = 0; i < N_BITMAPS; i++)
	{
		if (bitmapElementsDC[i] != NULL){
			SelectObject(bitmapElementsDC[i],bitmapElementsOrigin[i]);
			DeleteDC(bitmapElementsDC[i]);
		}
		if (bitmapElements[i] != NULL){
			DeleteObject(bitmapElements[i]);
		}
	}
	DeleteObject(hdcDoubleBuffer);
}