#include "Common.h"
#include "Labirinto.h"

extern Labirinto l;
//Jogador gClients[MAX_CLIENTS];
extern Jogador j;
HANDLE pipeClients[MAX_CLIENTS];

BOOL fim = FALSE;
int totalConnections = 0;