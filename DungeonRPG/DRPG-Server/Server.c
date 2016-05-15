#include "Server.h"
#include "Jogador.h"

TCHAR broadcastMessage[BUFFERSIZE];

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

	TCHAR buf[BUFFERSIZE];
	DWORD n;
	BOOL ret;

	ClientRequest pedido;

	//Limpar "strings" para receberem os primeiros pedidos
	memset(gClients[(int)param].resposta, TEXT('\0'), sizeof(TCHAR));	//Notificação privada
	memset(broadcastMessage, TEXT('\0'), sizeof(TCHAR));				//Notificação broadcast

	do {
		ret = ReadFile(hPipeCliente, &pedido, sizeof(ClientRequest), &n, NULL);
		if (!ret || !n) break;
		_tprintf(TEXT("[Server] Recebi %d bytes: \'%s\'... (ReadFile)\n"),n,pedido.msg);
		
		if (!start) {
			switch (pedido.command)
			{
			case GAMESTATUS:
				//swprintf(resposta.msg, "%d", totalConnections); //Clientes Ligados
				_tcscpy(gClients[(int)param].resposta, TEXT("Estatísticas dados pré-jogo...\n"));
				_swprintf(buf, TEXT("%d"), totalConnections);
				_tcscat(gClients[(int)param].resposta, TEXT("Total de jogadores ligados: "));
				_tcscat(gClients[(int)param].resposta, buf);
				break;

			case SETNAME:
				//Private Notification
				_tcscpy(gClients[(int)param].nome, pedido.msg);
				_tcscpy(gClients[(int)param].resposta, TEXT("Estás registado como: "));
				_tcscat(gClients[(int)param].resposta, pedido.msg);
				//Broadcast
				_tcscat(broadcastMessage, TEXT("Novo Cliente Ligado: "));
				_tcscat(broadcastMessage, gClients[(int)param].nome);
				break;

			case STARTGAME:
				start = TRUE;
				//broadcast
				_tcscat(broadcastMessage, TEXT("Novo jogo iniciado por: "));
				_tcscat(broadcastMessage, gClients[(int)param].nome);
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
					_tcscpy(gClients[(int)param].resposta, TEXT("Estatísticas de jogo a decorrer"));
					break;

				case SWITCH_STONE_AUTOHIT:
					if (gClients[(int)param].stoneAutoHit) {
						gClients[(int)param].stoneAutoHit = FALSE;
						_tcscpy(gClients[(int)param].resposta, TEXT("Stone AutoHit: Desligado!"));
					}
					else {
						gClients[(int)param].stoneAutoHit = TRUE;
						_tcscpy(gClients[(int)param].resposta, TEXT("Stone AutoHit: Ligado!"));
					}
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

DWORD WINAPI ActualizaClientes(LPVOID param){
	TCHAR buf[BUFFERSIZE];
	DWORD n;
	ServerResponse resposta;
	while (fim == FALSE && totalConnections > 0) {

		//prepara a "string" de resposta
		memset(resposta.msg, TEXT('\0'), sizeof(TCHAR));
		//Cortar "strings"
		if (broadcastMessage != TEXT('\0')) {
			_tcscpy(resposta.msg, broadcastMessage);
			memset(broadcastMessage, TEXT('\0'), sizeof(TCHAR));
		}
		if (gClients[(int)param].resposta != TEXT('\0')) {
			_tcscat(resposta.msg, TEXT('\n'));
			_tcscat(resposta.msg, gClients[(int)param].resposta);
			memset(gClients[(int)param].resposta, TEXT('\0'), sizeof(TCHAR));
		}
		//Para cada cliente
		for (int i = 0; i < totalConnections; i++) {
			if (gClients[i].hp > 0) {

				if (!start) SetEmptyMatrix(resposta.matriz); //security
				else UpdatePlayerLOS(gClients[i].x, gClients[i].y,resposta.matriz);

				if (!WriteFile(gClients[i].hPipe, &resposta, sizeof(ServerResponse), &n, NULL)) {
					_tperror(TEXT("[ERRO] Escrever no pipe... (WriteFile)\n"));
					exit(-1);
				}
			}
		}
		Sleep(1500);
		//Sleep(1000/15); //15 instantes por segundo
		//_tprintf(TEXT("[SERVER] Enviei %d bytes aos %d clientes... (WriteFile)\n"), n, totalConnections);
	}
}

//Disconnect from all pipes (Provavelmente não vai ser preciso)
void DesligarNamedPipes() {
	for (int i = 0; i < totalConnections; i++) {
		DisconnectNamedPipe(gClients[i].hPipe);
		_tprintf(TEXT("[ESCRITOR] Vou desligar o pipe... (CloseHandle)\n"));
		CloseHandle(gClients[i].hPipe);
	}
}