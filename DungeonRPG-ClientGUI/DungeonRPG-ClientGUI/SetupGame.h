#pragma once
#include "Common.h"
#include "GameProc.h"

extern HBITMAP	bitmapElements[N_BITMAPS];
extern HDC		bitmapElementsDC[N_BITMAPS];
extern HBITMAP	bitmapElementsOrigin[N_BITMAPS];

ATOM RegisterWndClassApp(HINSTANCE hInstance, TCHAR * szWinName);
HWND CreateWnd(HINSTANCE hInstance, TCHAR * szWinName);
int LoadGameResources();
void ConfigurarDCs(HDC hdcOrigin);
void UnlinkObjects();