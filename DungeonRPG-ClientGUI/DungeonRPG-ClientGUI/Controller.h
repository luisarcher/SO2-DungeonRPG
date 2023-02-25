#pragma once
#include "Common.h"

extern TCHAR receivedMSG[BUFFERSIZE];

DWORD EnviaTecla(int _command);
DWORD Registar(TCHAR * nome);
DWORD CallStartGame();