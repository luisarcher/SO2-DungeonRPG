#include "Server.h"
#include "Player.h"
#include "GameBoard.h"

TCHAR broadcastMessage[BUFFERSIZE];

/**
*	Thread que recebe e regista os novos clientes.
*/
DWORD WINAPI ListenForConnections(LPVOID param) {
	DWORD n;
	while (!fim && totalConnections < MAX_CLIENTS) {

		// PIPE TO HANDLE REQUESTS
		_tprintf(TEXT("[SERVER] Creating connection pipe for game stats: '%s' ... \n"), PIPE_NAME);
		if ((gClients[totalConnections].hPipe = CreateNamedPipe(
			PIPE_NAME, 
			PIPE_ACCESS_DUPLEX, 
			PIPE_WAIT | PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, 
			MAX_CLIENTS, 
			BUFFERSIZE * sizeof(TCHAR), 
			BUFFERSIZE * sizeof(TCHAR),
			1000, NULL)) == INVALID_HANDLE_VALUE){
			_tperror(TEXT("Error creating pipe for game stats!"));
			exit(-1);
		}

		// PIPE FOR SENDING THE GAME BOARD STATUS
		_tprintf(TEXT("[SERVER] Creating connection pipe for game map: '%s' ... \n"), PIPE_NAME_JOGO);
		if ((gClients[totalConnections].hPipeGame = CreateNamedPipe(
			PIPE_NAME_JOGO, 
			PIPE_ACCESS_OUTBOUND, 
			PIPE_WAIT | PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, 
			MAX_CLIENTS, 
			sizeof(ServerResponse), 
			sizeof(ServerResponse),
			1000, NULL)) == INVALID_HANDLE_VALUE) {
			_tperror(TEXT("Error creating pipe for game map!"));
			exit(-1);
		}

		// Waits for a client connection (ConnectNamedPipe - Blocking)
		_tprintf(TEXT("[SERVER] Waiting client connection ...\n"));
		if (!ConnectNamedPipe(gClients[totalConnections].hPipe, NULL)) {
			_tperror(TEXT("Error connecting on pipe for game stats!"));
			exit(-1);
		}

		if (!ConnectNamedPipe(gClients[totalConnections].hPipeGame, NULL)) {
			_tperror(TEXT("Error connecting on pipe for game map!"));
			exit(-1);
		}

		// Prepare new player data
		NewPlayer(totalConnections);

		// Handles a new client connection
		gClients[totalConnections].hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HandleClientConnection, (LPVOID)totalConnections, 0, &n);
		totalConnections++;
	}
	return 0;
}

/**
*	Thread que faz decorrer o tempo de jogo.
*	Sinaliza outras threads sempre que ocorre um "instante".
*/
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
				CheckItemDurability(&gClients[i]);
				CheckForThreats(&gClients[i]);
			}
	}
	return 0;
}

/**
*	Para cada cliente, fecha o handle da sua thread.
*/
void DesligarThreadsDeCliente() {
	for (int i = 0; i < totalConnections; i++)
		CloseHandle(gClients[i].hThread);
}

/**
*	Devolve o número de jogadores activos.
*/
int activePlayers() {
	int nPlayers = 0;
	for (int i = 0; i < totalConnections; i++)
		if (gClients[i].hp > 0) ++nPlayers;
	return nPlayers;
}

/**
* Analisa o pedido do cliente e envia uma resposta.
*/
DWORD WINAPI HandleClientConnection(LPVOID param) {
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
				_tcscpy(gClients[(int)param].name, pedido.msg);
				_tcscpy(respostaServidor, TEXT("Estás registado como: "));
				_tcscat(respostaServidor, pedido.msg);

				//Broadcast
				_tcscat(broadcastMessage, TEXT("Novo Cliente Ligado: "));
				_tcscat(broadcastMessage, gClients[(int)param].name);
				break;

			case STARTGAME:
				StartGame();
				_tcscpy(respostaServidor, TEXT("Começaste um novo jogo!"));

				//Broadcast
				_tcscat(broadcastMessage, TEXT("Novo jogo iniciado por: "));
				_tcscat(broadcastMessage, gClients[(int)param].name);
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
				MovePlayer((int)param, (pedido.command - 600));
				_tcscpy(respostaServidor, TEXT("Moving ..."));
			}
			else {
				switch (pedido.command)
				{
				case GAMESTATUS:
					_tcscpy(respostaServidor, TEXT("Estatísticas de jogo a decorrer"));
					break;
				case SWITCH_STONE_AUTOHIT:
					if (gClients[(int)param].stoneAutoHitToggle) {
						gClients[(int)param].stoneAutoHitToggle = FALSE;
						_tcscpy(respostaServidor, TEXT("Stone AutoHit: Disabled!"));
					}
					else {
						gClients[(int)param].stoneAutoHitToggle = TRUE;
						_tcscpy(respostaServidor, TEXT("Stone AutoHit: Enabled!"));
					}
					break;
				case STARTGAME:
					_tcscpy(respostaServidor, TEXT("The game already started!"));
					break;

				case QUITGAME:
					break;
				}
			} //fim if - mover
		} //fim if - start
		//Entrega a resposta final ao cliente
		_tprintf(TEXT("[Server] Sending %d bytes: \'%s\' to client [%d] (ReadFile)\n"), n, respostaServidor, (int)param);
		if (!WriteFile(hPipeCliente, respostaServidor, sizeof(TCHAR) * BUFFERSIZE, &n, NULL)) {
			_tperror(TEXT("[ERROR] Error replying to client... (WriteFile)\n"));
			exit(-1);
		}

		//Quando um jogador faz uma acção, os outros são notificados
		SetEvent(ghUpdateGameClientEvent);		//Sinalizar evento de difusão
	} while (pedido.command != QUITGAME);

	DropStones(&gClients[(int)param]);
	DisconnectPlayer(&gClients[(int)param]);

	//Broadcast
	_tcscat(broadcastMessage, gClients[(int)param].name);
	_tcscat(broadcastMessage, TEXT(" left the game."));

	_tprintf(TEXT("Thread %d exiting\n"), GetCurrentThreadId());
	return 0;
}

/**
*	Envia os dados de difusão a todos os clientes
*/
DWORD WINAPI ActualizaClientes(LPVOID param) {
	DWORD n;
	ServerResponse resposta;

	while (!fim) {

		//Espera que o evento de difusão seja sinalizado.
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
					//Enviar dados do jogador
					resposta.playerInfo = CopyPlayerData(gClients[i]);

					if (!start) SetEmptyMatrix(resposta.matriz); //security
					else UpdatePlayerLOS(gClients[i].x, gClients[i].y, resposta.matriz);
					if (!WriteFile(gClients[i].hPipeGame, &resposta, sizeof(ServerResponse), &n, NULL)) {
						_tprintf(TEXT("[ERROR] Error writing to client %d:%s\n"), gClients[i].id, gClients[i].name);
						gClients[i].hp = 0;
					}
				}
			}//for ends
			_tprintf(TEXT("[SERVER] Enviei %d bytes aos %d clientes... (WriteFile)\n"), (int)n, activePlayers());
			
			//_tprintf(TEXT("Shared Memory:\n"));
			//CopyGameStateToSharedMemory();
		}
		//Não existem jogadores activos? Então não está nenhum jogo a decorrer
		else start = FALSE;
	}
	_tprintf(TEXT("Thread %d exiting\n"), GetCurrentThreadId());
	return 0;
}

void StartGame() {
	start = TRUE;
	for (int i = 0; i < totalConnections; i++)
		gClients[i].hp = BASE_HP;
}

/**
*	Copia dados do labirinto para a memória partilhada.
*/
/*void CopyGameStateToSharedMemory() {
	Labirinto * l = shLabirinto;
	for (int i = 0; i < LABIRINTOSIZE; i++)
	{
		for (int j = 0; j < LABIRINTOSIZE; j++)
		{
			l->labirinto[i][j] = gLabirinto.labirinto[i][j];
		}
	}
}*/
