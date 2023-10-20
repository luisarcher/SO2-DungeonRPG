#pragma once
#include "Common.h"

extern TCHAR receivedMSG[BUFFERSIZE];

DWORD SendCommand(int _command);
DWORD Register(TCHAR * nome);
DWORD CallStartGame();