#pragma once
#include "Common.h"

extern HDC hdcDoubleBuffer;
//extern HBITMAP	hBitmapDoubleBuffer;
//extern HBITMAP	hBitmapDoubleBufferOrigin;

LRESULT CALLBACK WinProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
