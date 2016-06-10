#pragma once
#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include "lib\ClientDLL.h"

#define LABIRINTOSIZE		70
#define TILE_SZ				64
#define BOARD_TOP_MARGIN	25
#define BOARD_LEFT_MARGIN	25

//Common
extern HINSTANCE ghInstance;

//Connect
extern HANDLE hPipe;
extern HANDLE hPipeJogo;