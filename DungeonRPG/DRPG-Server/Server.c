#include "Server.h"
#include "Jogador.h"

TCHAR broadcastMessage[BUFFERSIZE];

//SignUp and connect clients
DWORD WINAPI RecebeClientes(LPVOID param) {
	DWORD n;
	while (!fim && totalConnections < MAX_CLIENTS) {

		//Pipe para tratar pedidos
		_tprintf(TEXT("[SERVER] Vou passar � cria��o de uma c�pia do pipe '%s' ... \n"), PIPE_NAME);
		if ((gClients[totalConnections].hPipe = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_WAIT | PIPE_TYPE_MESSAGE
			| PIPE_READMODE_MESSAGE, MAX_CLIENTS, BUFFERSIZE * sizeof(TCHAR), BUFFERSIZE * sizeof(TCHAR),
			1000, NULL)) == INVALID_HANDLE_VALUE){
			_tperror(TEXT("Erro ao criar named pipe!"));
			exit(-1);
		}

		//Pipe para jogo
		_tprintf(TEXT("[SERVER] Vou passar � cria��o de uma c�pia do pipe '%s' ... \n"), PIPE_NAME_JOGO);
		if ((gClients[totalConnections].hPipeJogo = CreateNamedPipe(PIPE_NAME_JOGO, PIPE_ACCESS_OUTBOUND, PIPE_WAIT | PIPE_TYPE_MESSAGE
			| PIPE_READMODE_MESSAGE, MAX_CLIENTS, sizeof(ServerResponse), sizeof(ServerResponse),
			1000, NULL)) == INVALID_HANDLE_VALUE) {
			_tperror(TEXT("Erro ao criar named pipe para o jogo!"));
			exit(-1);
		}

		//Espera a liga��o de um cliente (ConnectNamedPipe � bloqueante)
		_tprintf(TEXT("[SERVER] Esperar liga��o de um cliente...\n"));
		if (!ConnectNamedPipe(gClients[totalConnections].hPipe, NULL)) {
			_tperror(TEXT("Erro na liga��o!"));
			exit(-1);
		}

		if (!ConnectNamedPipe(gClients[totalConnections].hPipeJogo, NULL)) {
			_tperror(TEXT("Erro na liga��o!"));
			exit(-1);
		}

		NovoJogador(totalConnections); //prepara os dados do jogador
		//Atende o cliente que se ligou
		gClients[totalConnections].hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtendeCliente, (LPVOID)totalConnections, 0, &n);
		totalConnections++;
	}
	//DesligarNamedPipes(); //depois de fim //VERIFICAR!!
	return 0;
}

DWORD WINAPI AtendeCliente(LPVOID param) {
	//ide buscar o handle do cliente que est� no array global
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
				_tcscpy(resposta.msg, TEXT("Estat�sticas dados pr�-jogo...\n"));
				_swprintf(buf, TEXT("%d"), totalConnections);
				_tcscat(resposta.msg, TEXT("Total de jogadores ligados: "));
				_tcscat(resposta.msg, buf);
				ret = WriteFile(hPipeCliente, &resposta, sizeof(ServerResponse), &n, NULL);
				break;
			case SETNAME:
				_tcscpy(gClients[(int)param].nome, pedido.msg);
				_tcscpy(resposta.msg, TEXT("Est�s registado como: "));
				_tcscat(resposta.msg, pedido.msg);
				ret = WriteFile(hPipeCliente, &resposta, sizeof(ServerResponse), &n, NULL);

				//Broadcast
				_tcscat(broadcastMessage, TEXT("Novo Cliente Ligado: "));
				_tcscat(broadcastMessage, gClients[(int)param].nome);
				break;

			case STARTGAME:
				start = TRUE;
				_tcscpy(resposta.msg, TEXT("Come�aste um novo jogo!"));
				ret = WriteFile(hPipeCliente, &resposta, sizeof(ServerResponse), &n, NULL);
				//broadcast -> Isto tem de levar um WaitFor
				_tcscat(broadcastMessage, TEXT("Novo jogo iniciado por: "));
				_tcscat(broadcastMessage, gClients[(int)param].nome);
				//WaitForMultipleObjects()
				break;

			case QUITGAME:
				break;

			default:
				break;
			}
		}
		else { //O jogo est� a decorrer
			if (pedido.command == MOVEUP || pedido.command == MOVEDOWN
				|| pedido.command == MOVELEFT || pedido.command == MOVERIGHT)
				MoverJogador((int)param,(pedido.command - 600));
			else {
				switch (pedido.command)
				{
				case GAMESTATUS:
					_tcscpy(resposta.msg, TEXT("Estat�sticas de jogo a decorrer"));
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
	DisconnectNamedPipe(gClients[(int)param].hPipeJogo);
	CloseHandle(hPipeCliente);
	CloseHandle(gClients[(int)param].hPipeJogo);

	_tprintf(TEXT("[SERVIDOR] Cliente [%d] desligou-se!\n"), gClients[(int)param].id);
	gClients[(int)param].hp = 0;
	//deixa cair pedras
	return 0;
}

//Disconnect from all pipes (Provavelmente n�o vai ser preciso, s� para as threads)
void DesligarNamedPipes() {
	for (int i = 0; i < totalConnections; i++) {
		DisconnectNamedPipe(gClients[i].hPipe);
		_tprintf(TEXT("[ESCRITOR] Vou desligar o pipe... (CloseHandle)\n"));
		CloseHandle(gClients[i].hPipe);
	}
}

void DesligarThreadsDeCliente() {
	for (int i = 0; i < totalConnections; i++) {
		CloseHandle(gClients[i].hThread);
	}
}

DWORD WINAPI ActualizaClientes(LPVOID param) {
	TCHAR buf[BUFFERSIZE];
	DWORD n;
	ServerResponse resposta;

	int activePlayers = 0;

	while (!fim) {
		for (size_t i = 0; i < totalConnections; i++)
			if (gClients[i].hp > 0) ++activePlayers;
		
		if (totalConnections > 0 && activePlayers > 0) {
			activePlayers = 0;
			//prepara a "string" de resposta
			memset(resposta.msg, TEXT('\0'), sizeof(TCHAR));
			if (broadcastMessage != TEXT('\0')) {
				_tcscpy(resposta.msg, broadcastMessage);
				memset(broadcastMessage, TEXT('\0'), sizeof(TCHAR));
			}
			//Para cada cliente
			for (int i = 0; i < totalConnections; i++) {
				if (gClients[i].hp > 0) {

					//Envia a matriz para os jogadores
					if (!start) SetEmptyMatrix(&resposta.matriz); //security
					else UpdatePlayerLOS(gClients[i].x, gClients[i].y, &resposta.matriz, i);
					if (!WriteFile(gClients[i].hPipeJogo, &resposta, sizeof(ServerResponse), &n, NULL)) {
						_tprintf(TEXT("[ERRO] Ao escrever no pipe do cliente %d:%s\n"), gClients[i].id, gClients[i].nome);
						gClients[i].hp = 0;
					}

					//Player Activity //Mutex
					RecoverPlayerStamina(&gClients[i]);
					AttackClosePlayers(&gClients[i]);

				}
			}//fim for
			_tprintf(TEXT("[SERVER] Enviei %d bytes aos %d clientes... (WriteFile)\n"), n, totalConnections);
		}
		//Sleep(1000);
		Sleep(1000 / 15); //15 instantes por segundo
	}
}
