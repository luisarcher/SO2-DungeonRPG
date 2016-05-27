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
	int lentidao;

	int lentidaoCounter;
	int atkCounter;

	BOOL stoneAutoHit;
	HANDLE hPipe;
	HANDLE hPipeJogo;
	HANDLE hThread;
	TCHAR resposta[BUFFERSIZE];
};

void NovoJogador(int id);
void DesligarJogador(Jogador * j);
void MoverJogador(int playerId, int keystroke);
void UpdatePlayerLOS(int x, int y, int(*matriz)[PLAYER_LOS]);
void SetEmptyMatrix(int(*matriz)[PLAYER_LOS]);

void SetPlayerInRandomPosition(Jogador * p);
void AskPlayerToCollectItems(Jogador * p);
void RecoverPlayerStamina(Jogador * p);
void AttackClosePlayers(Jogador * p);
void DropStones(Jogador * p);
BOOL UseStone(Jogador * p);

BOOL hasPlayerIn(int x, int y);
BOOL hasMonsterAndPlayerIn(int x, int y);
BOOL hasObjectIn(int x, int y);
BOOL hasWallIn(int x, int y);
BOOL hasStamina(Jogador p);

extern Jogador gClients[MAX_CLIENTS];
#endif