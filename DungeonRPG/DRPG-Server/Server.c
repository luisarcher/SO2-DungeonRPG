#include "Server.h"
#include "Jogador.h"

//SignUp and connect clients
DWORD WINAPI RecebeClientes(LPVOID param) {
	DWORD n;
	while (!fim && totalConnections < MAX_CLIENTS) {

		_tprintf(TEXT("[SERVER] Vou passar à criação de uma cópia do pipe '%s' ... \n"), PIPE_NAME);
		if ((gClients[totalConnections].hPipe = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_WAIT | PIPE_TYPE_MESSAGE
			| PIPE_READMODE_MESSAGE, MAX_CLIENTS, BUFFERSIZE * sizeof(TCHAR), BUFFERSIZE * sizeof(TCHAR),
			1000, NULL)) == INVALID_HANDLE_VALUE){
			_tperror(TEXT("Erro ao criar named pipe!"));
			exit(-1);
		}

		//Espera a ligação de um cliente (ConnectNamedPipe é bloqueante)
		_tprintf(TEXT("[SERVER] Esperar ligação de um cliente...\n"));
		if (!ConnectNamedPipe(gClients[totalConnections].hPipe, NULL)) {
			_tperror(TEXT("Erro na ligação!"));
			exit(-1);
		}

		//Atende o cliente que se ligou
		gClients[totalConnections].hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtendeCliente, (LPVOID)totalConnections, 0, &n);
		NovoJogador(&gClients[totalConnections]); //prepara os dados do jogador
		totalConnections++;
	}
	return 0;
}

DWORD WINAPI AtendeCliente(LPVOID param) {
	//ide buscar o handle do cliente que está no array global
	HANDLE hPipeCliente = gClients[(int)param].hPipe;

	TCHAR buf[256];
	DWORD n;
	BOOL ret;

	ClientRequest pedido;
	ServerResponse resposta;

	do {
		ret = ReadFile(hPipeCliente, &pedido, sizeof(ClientRequest), &n, NULL);
		if (!ret || !n) break;
		_tprintf(TEXT("[Server] Recebi %d bytes: \'%s\'... (ReadFile)\n"),n,pedido.msg);
		memset(resposta.msg, '\0', sizeof(TCHAR)); //prepara a "string" de resposta
		memset(gClients[(int)param].resposta, '\0', sizeof(TCHAR));
		if (!start) {
			switch (pedido.command)
			{
			case GAMESTATUS:
				//swprintf(resposta.msg, "%d", totalConnections); //Clientes Ligados
				_tcscpy(resposta.msg, TEXT("Estatísticas dados pré-jogo...\n"));
				_swprintf(buf, TEXT("%d"), totalConnections);
				_tcscat(resposta.msg, TEXT("Total de jogadores ligados: "));
				_tcscat(resposta.msg, buf);
				ret = WriteFile(hPipeCliente, &resposta, sizeof(ServerResponse), &n, NULL);
				break;
			case SETNAME:
				_tcscpy(gClients[(int)param].nome, pedido.msg);
				_tcscpy(resposta.msg, TEXT("Estás registado como: "));
				_tcscat(resposta.msg, pedido.msg);
				ret = WriteFile(hPipeCliente, &resposta, sizeof(ServerResponse), &n, NULL);
				break;

			case STARTGAME:
				start = TRUE;
				_tcscpy(resposta.msg, TEXT("Começaste um novo jogo!"));
				ret = WriteFile(hPipeCliente, &resposta, sizeof(ServerResponse), &n, NULL);
				break;

			case QUITGAME:
				break;

			default:
				break;
			}
		}
		else { //O jogo está a decorrer
			if (pedido.command == MOVEUP || pedido.command == MOVEDOWN
				|| pedido.command == MOVELEFT || pedido.command == MOVERIGHT)
				MoverJogador((int)param,(pedido.command - 600));
			else {
				switch (pedido.command)
				{
				case GAMESTATUS:
					_tcscpy(resposta.msg, TEXT("Estatísticas de jogo a decorrer"));
					ret = WriteFile(hPipeCliente, &resposta, sizeof(ServerResponse), &n, NULL);
					break;
				case SWITCH_STONE_AUTOHIT:
					if (gClients[(int)param].stoneAutoHit) {
						gClients[(int)param].stoneAutoHit = FALSE;
						_tcscpy(resposta.msg, TEXT("Stone AutoHit: Desligado!"));
					}
					else {
						gClients[(int)param].stoneAutoHit = TRUE;
						_tcscpy(resposta.msg, TEXT("Stone AutoHit: Ligado!"));
					}
					ret = WriteFile(hPipeCliente, &resposta, sizeof(ServerResponse), &n, NULL);
					break;

				case QUITGAME:
					break;
				}
			} //fimif - mover
		} //fimif - start
		if (!ret) {
			_tperror(TEXT("[ERRO] Escrever no pipe... (WriteFile)\n"));
			exit(-1);
		}
	} while (pedido.command != QUITGAME);
	DisconnectNamedPipe(hPipeCliente);
	CloseHandle(hPipeCliente);
	_tprintf(TEXT("[SERVIDOR] Cliente [%d] desligou-se!\n"), (int)hPipeCliente);
	return 0;
}

//Disconnect from all pipes (Provavelmente não vai ser preciso, só para as threads)
void DesligarNamedPipes() {
	for (int i = 0; i < totalConnections; i++) {
		DisconnectNamedPipe(gClients[i].hPipe);
		_tprintf(TEXT("[ESCRITOR] Vou desligar o pipe... (CloseHandle)\n"));
		CloseHandle(gClients[i].hPipe);
	}
}

DWORD WINAPI ActualizaClientes(LPVOID param){
	TCHAR buf[BUFFERSIZE];
	DWORD n;
	
	//do {
		/*_tcscpy(buf, TEXT("Isto é o jogo vindo do servidor"));
		//Escrever para todos os clientes inscritos
		for (int i = 0; i < totalConnections; i++){
			if (!WriteFile(gClients[i].hPipe, buf, _tcslen(buf)*sizeof(TCHAR), &n, NULL)) {
				_tperror(TEXT("[ERRO] Escrever no pipe... (WriteFile)\n"));
				exit(-1);
			}
		}*/

		/*_tprintf(TEXT("[SERVER] Enviei %d bytes aos %d clientes... (WriteFile)\n"), n, totalConnections);
	} while (_tcsncmp(buf, TEXT("fim"), 3));*/
}