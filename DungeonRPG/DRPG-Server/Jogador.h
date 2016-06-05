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
	int itemDurationCounter;

	BOOL stoneAutoHit;
	HANDLE hPipe;
	HANDLE hPipeJogo;
	HANDLE hThread;
};

void NovoJogador(int id);
void DesligarJogador(Jogador * j);
void MoverJogador(int playerId, int keystroke);
void UpdatePlayerLOS(int x, int y, int(*matriz)[PLAYER_LOS]);
void SetEmptyMatrix(int(*matriz)[PLAYER_LOS]);

void SetPlayerInRandomPosition(Jogador * p);
void AskPlayerToCollectItems(Jogador * p);
void RecoverPlayerStamina(Jogador * p);
void CheckItemDurability(Jogador * p);
void AttackClosePlayers(Jogador * p);
void CheckForThreats(Jogador* p);
void DropStones(Jogador * p);
BOOL UseStone(Jogador * p);
BOOL hasStamina(Jogador p);

extern Jogador gClients[MAX_CLIENTS];
#endif