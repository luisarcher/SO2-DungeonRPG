#pragma once
#include "Common.h"
#include "GameProc.h"

extern HBITMAP	bitmapElements[N_BITMAPS];
extern HDC		bitmapElementsDC[N_BITMAPS];
extern HBITMAP	bitmapElementsOrigin[N_BITMAPS];

ATOM RegistaClasse(HINSTANCE hInstance, TCHAR * szWinName);
HWND CriarJanela(HINSTANCE hInstance, TCHAR * szWinName);
int CarregarTodasAsImagens();
void ConfigurarDCs(HDC hdcOrigin);
void UnlinkObjects();