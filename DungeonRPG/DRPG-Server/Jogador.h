#ifndef JOGADOR_H_INCLUDED
#define JOGADOR_H_INCLUDED
#include "Common.h"

typedef struct JOGADOR Jogador;
struct JOGADOR {
	TCHAR nome[20];
	int x;
	int y;
	int id;
	int hp;
	int nStones;
	BOOL stoneAutoHit;
	HANDLE hPipe;
	HANDLE hThread;
	TCHAR resposta[BUFFERSIZE]; //Resposta que será dada ao cliente no próximo envio
};

void NovoJogador(Jogador * j);
int MoverJogador(int playerId, int keystroke);
void UpdatePlayerLOS(int x, int y, int(*matriz)[PLAYER_LOS]);
void SetEmptyMatrix(int(*matriz)[PLAYER_LOS]);

extern Jogador gClients[MAX_CLIENTS];
#endif