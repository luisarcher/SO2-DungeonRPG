#include "Server.h"
#include "Jogador.h"

TCHAR broadcastMessage[BUFFERSIZE];

//SignUp and connect clients
DWORD WINAPI RecebeClientes(LPVOID param) {
	DWORD n;
	while (!fim && totalConnections < MAX_CLIENTS) {

		//PIPE TO HANDLE REQUESTS
		_tprintf(TEXT("[SERVER] Vou passar à criação de uma cópia do pipe '%s' ... \n"), PIPE_NAME);
		if ((gClients[totalConnections].hPipe = CreateNamedPipe(
			PIPE_NAME, 
			PIPE_ACCESS_DUPLEX, 
			PIPE_WAIT | PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, 
			MAX_CLIENTS, 
			BUFFERSIZE * sizeof(TCHAR), 
			BUFFERSIZE * sizeof(TCHAR),
			1000, NULL)) == INVALID_HANDLE_VALUE){
			_tperror(TEXT("Erro ao criar named pipe!"));
			exit(-1);
		}

		//PIPE FOR SENDING GAME BOARD
		_tprintf(TEXT("[SERVER] Vou passar à criação de uma cópia do pipe '%s' ... \n"), PIPE_NAME_JOGO);
		if ((gClients[totalConnections].hPipeJogo = CreateNamedPipe(
			PIPE_NAME_JOGO, 
			PIPE_ACCESS_OUTBOUND, 
			PIPE_WAIT | PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, 
			MAX_CLIENTS, 
			sizeof(ServerResponse), 
			sizeof(ServerResponse),
			1000, NULL)) == INVALID_HANDLE_VALUE) {
			_tperror(TEXT("Erro ao criar named pipe para o jogo!"));
			exit(-1);
		}

		//Espera a ligação de um cliente (ConnectNamedPipe é bloqueante)
		_tprintf(TEXT("[SERVER] Esperar ligação de um cliente...\n"));
		if (!ConnectNamedPipe(gClients[totalConnections].hPipe, NULL)) {
			_tperror(TEXT("Erro na ligação!"));
			exit(-1);
		}

		if (!ConnectNamedPipe(gClients[totalConnections].hPipeJogo, NULL)) {
			_tperror(TEXT("Erro na ligação!"));
			exit(-1);
		}

		NovoJogador(totalConnections); //prepara os dados do novo jogador
		//Atende o cliente que se ligou
		gClients[totalConnections].hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtendeCliente, (LPVOID)totalConnections, 0, &n);
		totalConnections++;
	}
	return 0;
}

DWORD WINAPI GameTimer(LPVOID param){
	while (!fim) {
		ResetEvent(ghGameInstanceEvent);	//Reseta o evento para o estado inicial
		Sleep(GAME_INSTANCE_TIME);			//Passa um instante de jogo
		SetEvent(ghGameInstanceEvent);		//Sinaliza o evento
	}
	return 0;
}

/**
* Por cada instante,
* Recuperar 1 ponto de stamina de cada jogador.
* Se existir algum jogador nas proximidadades, ataca-o.
*/
DWORD WINAPI GameEvents(LPVOID param) {
	while (!fim) {
		WaitForSingleObject(ghGameInstanceEvent, INFINITE);		//Espera pelo evento ser sinalizado
		for (int i = 0; i < totalConnections; i++)
			if (gClients[i].hp > 0) { //is active?
				RecoverPlayerStamina(&gClients[i]);
				AttackClosePlayers(&gClients[i]);
			}
	}
	return 0;
}

/**
* Analisa o pedido do cliente e envia uma resposta.
*/
DWORD WINAPI AtendeCliente(LPVOID param) {
	HANDLE hPipeCliente = gClients[(int)param].hPipe;

	TCHAR buf[BUFFERSIZE];
	TCHAR respostaServidor[BUFFERSIZE];
	DWORD n;
	BOOL ret;

	ClientRequest pedido;

	do {
		ret = ReadFile(hPipeCliente, &pedido, sizeof(ClientRequest), &n, NULL);
		if (!ret || !n) break;
		_tprintf(TEXT("[Server] Recebi %d bytes: \'%s\'... (ReadFile)\n"),n,pedido.msg);
		memset(respostaServidor, '\0', sizeof(TCHAR) * BUFFERSIZE); //prepara a "string" de resposta

		if (!start) {
			switch (pedido.command)
			{
			case GAMESTATUS:
				//swprintf(resposta.msg, "%d", totalConnections); //Clientes Ligados
				_tcscpy(respostaServidor, TEXT("Estatísticas dados pré-jogo...\n"));
				_swprintf(buf, TEXT("%d"), totalConnections);
				_tcscat(respostaServidor, TEXT("Total de jogadores ligados: "));
				_tcscat(respostaServidor, buf);
				break;
			case SETNAME:
				_tcscpy(gClients[(int)param].nome, pedido.msg);
				_tcscpy(respostaServidor, TEXT("Estás registado como: "));
				_tcscat(respostaServidor, pedido.msg);

				//Broadcast
				_tcscat(broadcastMessage, TEXT("Novo Cliente Ligado: "));
				_tcscat(broadcastMessage, gClients[(int)param].nome);
				break;

			case STARTGAME:
				start = TRUE;
				_tcscpy(respostaServidor, TEXT("Começaste um novo jogo!"));

				//Broadcast
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
				|| pedido.command == MOVELEFT || pedido.command == MOVERIGHT) {
				MoverJogador((int)param, (pedido.command - 600));
				_tcscpy(respostaServidor, TEXT("A mover..."));
			}
			else {
				switch (pedido.command)
				{
				case GAMESTATUS:
					_tcscpy(respostaServidor, TEXT("Estatísticas de jogo a decorrer"));
					break;
				case SWITCH_STONE_AUTOHIT:
					if (gClients[(int)param].stoneAutoHit) {
						gClients[(int)param].stoneAutoHit = FALSE;
						_tcscpy(respostaServidor, TEXT("Stone AutoHit: Desligado!"));
					}
					else {
						gClients[(int)param].stoneAutoHit = TRUE;
						_tcscpy(respostaServidor, TEXT("Stone AutoHit: Ligado!"));
					}
					break;
				case STARTGAME:
					_tcscpy(respostaServidor, TEXT("O jogo já começou!"));
					break;

				case QUITGAME:
					break;
				}
			} //fim if - mover
		} //fim if - start
		//Entrega a resposta final ao cliente
		_tprintf(TEXT("[Server] A responder com %d bytes: \'%s\' ao cliente [%d] (ReadFile)\n"), n, respostaServidor, (int)param);
		if (!WriteFile(hPipeCliente, respostaServidor, sizeof(TCHAR) * BUFFERSIZE, &n, NULL)) {
			_tperror(TEXT("[ERRO] Responder ao cliente... (WriteFile)\n"));
			exit(-1);
		}

		SetEvent(ghUpdateGameClientEvent);		//Sinalizar evento de difusão
	} while (pedido.command != QUITGAME);

	DropStones(&gClients[(int)param]);
	DesligarJogador(&gClients[(int)param]);

	//Broadcast
	_tcscat(broadcastMessage, gClients[(int)param].nome);
	_tcscat(broadcastMessage, TEXT(" saiu do jogo."));

	_tprintf(TEXT("Thread %d exiting\n"), GetCurrentThreadId());
	return 0;
}

void DesligarThreadsDeCliente() {
	for (int i = 0; i < totalConnections; i++)
		CloseHandle(gClients[i].hThread);
}

int activePlayers() {
	int nPlayers = 0;
	for (int i = 0; i < totalConnections; i++)
		if (gClients[i].hp > 0) ++nPlayers;
	return nPlayers;
}

DWORD WINAPI ActualizaClientes(LPVOID param) {
	DWORD n;
	ServerResponse resposta;

	while (!fim) {
		WaitForSingleObject(ghUpdateGameClientEvent, INFINITE);

		if (totalConnections > 0 && activePlayers() > 0) {
			//Concatena a mensagem de difusão para todos os clientes
			memset(resposta.msg, TEXT('\0'), sizeof(TCHAR) * BUFFERSIZE);
			if (broadcastMessage != TEXT('\0')) {
				_tcscpy(resposta.msg, broadcastMessage);
				memset(broadcastMessage, TEXT('\0'), sizeof(TCHAR) * BUFFERSIZE);
			}

			//Actualiza o labirinto de todos os clientes activos
			for (int i = 0; i < totalConnections; i++) {
				if (gClients[i].hp > 0) {
					if (!start) SetEmptyMatrix(&resposta.matriz); //security
					else UpdatePlayerLOS(gClients[i].x, gClients[i].y, &resposta.matriz);
					if (!WriteFile(gClients[i].hPipeJogo, &resposta, sizeof(ServerResponse), &n, NULL)) {
						_tprintf(TEXT("[ERRO] Ao escrever no pipe do cliente %d:%s\n"), gClients[i].id, gClients[i].nome);
						gClients[i].hp = 0;
					}
				}
			}//for ends
			_tprintf(TEXT("[SERVER] Enviei %d bytes aos %d clientes... (WriteFile)\n"), (int)n, activePlayers());
		}
	}
	_tprintf(TEXT("Thread %d exiting\n"), GetCurrentThreadId());
	return 0;
}
