#include "Common.h"
#include "Labirinto.h"

Labirinto l;
Jogador gClients[MAX_CLIENTS];

HANDLE pipeClients[MAX_CLIENTS];
HANDLE hconsola;

extern BOOL fim = FALSE;
extern int totalConnections = 0;