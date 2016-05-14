#include "Configuration.h"
using namespace std;

DWORD WINAPI RecebeClientes(LPVOID param);
DWORD WINAPI AtendeCliente(LPVOID param);

void DesligarNamedPipes();
void ServerBroadcasting();