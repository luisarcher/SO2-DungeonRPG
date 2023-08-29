#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "Common.h"

//Estrutura para uso interno
typedef struct Player {
	TCHAR name[20];
	int x;
	int y;
	int id;
	int hp;
	int nStones;
	int movSpeed;

	int speedCounter;
	int atkCounter;
	int itemDurationCounter;

	BOOL stoneAutoHitToggle;
	HANDLE hPipe;
	HANDLE hPipeGame;
	HANDLE hThread;
} Player;

//Estrutura para informar o jogador
//Dedicada para não enviar toda a estrutura com dados críicos ao server
typedef struct {
	int x;
	int y;
	int hp;
	int nStones;
	int speedCounter;
	int atkCounter;
	int itemDurationCounter;
	BOOL stoneAutoHitToggle;
}PlayerInformation;

void NewPlayer(int id);
void DisconnectPlayer(Player* j);

void MovePlayer(int playerId, int keystroke);
void UpdatePlayerLOS(int x, int y, int(*matriz)[PLAYER_LOS]);
void SetEmptyMatrix(int(*matriz)[PLAYER_LOS]);

void SetPlayerInRandomPosition	(	Player* p);
void AskPlayerToCollectItems	(	Player* p);
void RecoverPlayerStamina		(	Player* p);
void CheckItemDurability		(	Player* p);
void AttackClosePlayers			(	Player* p);
void CheckForThreats			(	Player* p);
void DropStones					(	Player* p);
BOOL UseStone					(	Player* p);
BOOL hasStamina					(	Player  p);

PlayerInformation CopyPlayerData(Player p);

extern Player gClients[MAX_CLIENTS];
#endif