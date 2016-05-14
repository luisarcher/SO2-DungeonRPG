#include "Common.h"

HANDLE pipeClients[MAX_CLIENTS];
HANDLE hconsola;

extern BOOL fim = FALSE;
extern int totalConnections = 0;