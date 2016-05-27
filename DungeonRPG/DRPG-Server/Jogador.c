#include "Jogador.h"
#include "Labirinto.h"
#include "Common.h"
#include "Server.h"

void NovoJogador(int id) {
	Jogador * j = &gClients[id];
	_tcscpy(j->nome, TEXT("guest"));
	j->hp = (int)HP_BASE;
	j->lentidao = (int)LENTIDAO_BASE;
	j->id = id;
	j->nStones = 0;
	j->stoneAutoHit = TRUE;

	j->lentidaoCounter = 0;
	j->atkCounter = 0;
	j->itemDurationCounter = 0;

	//Define x e y do jogador (pos vazia) e regista-o no labirinto
	SetPlayerInRandomPosition(j);
}

void DesligarJogador(Jogador * j) {
	j->hp = 0;
	DisconnectNamedPipe(j->hPipe);
	DisconnectNamedPipe(j->hPipeJogo);
	CloseHandle(j->hPipe);
	CloseHandle(j->hPipeJogo);
	_tprintf(TEXT("[SERVIDOR] Cliente [%d] desligou-se!\n"), j->id);
}

void MoverJogador(int playerId, int keystroke) {
	Jogador *j = &gClients[playerId];

	if (!hasStamina(*j)) return;

	WaitForSingleObject(gMutexLabirinto, INFINITE);
	switch (keystroke) {
		case KEY_UP:
		{
			if (j->y > 1)
			{
				gLabirinto.labirinto[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (gLabirinto.labirinto[j->y][j->x] / 100) : EMPTY);
				if (!hasWallIn(j->x, j->y - 1) && !hasPlayerIn(j->x, j->y - 1)) j->y--;
				_tprintf(TEXT("Moving up...\n"));
			}
			break;
		}
		case KEY_DOWN:
		{
			if (j->y < LABIRINTOSIZE-2)
			{
				gLabirinto.labirinto[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (gLabirinto.labirinto[j->y][j->x] / 100) : EMPTY);
				if (!hasWallIn(j->x, j->y + 1) && !hasPlayerIn(j->x, j->y + 1))j->y++;
				_tprintf(TEXT("Moving down...\n"));
			}
			break;
		}
		case KEY_LEFT:
		{
			if (j->x > 1)
			{
				gLabirinto.labirinto[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (gLabirinto.labirinto[j->y][j->x] / 100) : EMPTY);
				if (!hasWallIn(j->x - 1, j->y) && !hasPlayerIn(j->x - 1, j->y)) j->x--;
				_tprintf(TEXT("Moving left...\n"));
			}
			break;
		}
		case KEY_RIGHT:
		{
			if (j->x < LABIRINTOSIZE - 2)
			{
				gLabirinto.labirinto[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (gLabirinto.labirinto[j->y][j->x] / 100) : EMPTY);
				if (!hasWallIn(j->x + 1, j->y) && !hasPlayerIn(j->x + 1, j->y)) j->x++;
				_tprintf(TEXT("Moving right...\n"));
			}
			break;
		}
		default:
		{
			_tprintf(TEXT("[SERVER] Não foi possível movimentar o jogador!"));
			break;
		}
	}
	if (hasObjectIn(j->x, j->y)) AskPlayerToCollectItems(j);
	// Update matrix after collected items
	gLabirinto.labirinto[j->y][j->x] = playerId;
	ReleaseMutex(gMutexLabirinto);
	// Player is now "tired" and recovering stamina
	j->lentidaoCounter = j->lentidao; //player is able to move on 0
	_tprintf(TEXT("P[%d] -> POSX: %d POSY: %d\n\n"),playerId, j->x, j->y);
}

void UpdatePlayerLOS(int x, int y, int (*matriz)[PLAYER_LOS]) {
	//validar o scroll
	// - Definir margens e encostar o scroll ao mapa
	SetEmptyMatrix(matriz);
	
	int iniX, iniY, maxX, maxY;

	if (x - 5 < 0) iniX = 0;
	else iniX = x - 5;
	if (x + 5 > LABIRINTOSIZE) maxX = LABIRINTOSIZE;
	else maxX = x + 5;

	if (y - 5 < 0) iniY = 0;
	else iniY = y - 5;
	if (y + 5 > LABIRINTOSIZE) maxY = LABIRINTOSIZE;
	else maxY = y + 5;

	int m=0, n= 0;
	for (int i = iniY; i < maxY; i++, n++, m=0)
	{
		for (int j = iniX; j < maxX; j++, m++)
		{
			matriz[n][m] = gLabirinto.labirinto[i][j];
		}
	}
}

/**
*	Preenche a matriz do jogador com "novoeiro".
*/
void SetEmptyMatrix(int (*matriz)[PLAYER_LOS]) {
	for (int i = 0; i < PLAYER_LOS; i++)
		for (int j = 0; j < PLAYER_LOS; j++)
			matriz[i][j] = FOG_OF_WAR;
}

/**
*	Coloca o jogador numa posição aleatória no labirinto que esteja vazia.
*/
void SetPlayerInRandomPosition(Jogador * p) {
	int x, y;
	do {
		srand(time(NULL));
		x = (rand() % LABIRINTOSIZE);
		y = (rand() % LABIRINTOSIZE);
	} while (!(gLabirinto.labirinto[y][x] == EMPTY));

	p->x = x;
	p->y = y;
	gLabirinto.labirinto[y][x] = p->id;
}

/**
*	O jogador recupera um ponto de stamina.
*/
void RecoverPlayerStamina(Jogador * p) {
	if (p->lentidaoCounter > 0)
		--p->lentidaoCounter;
	if (p->atkCounter > 0)
		--p->atkCounter;
}

void CheckItemDurability(Jogador * p) {
	if (p->itemDurationCounter > 0)
		--p->itemDurationCounter;
	else {
		if (p->lentidao < LENTIDAO_BASE)
			p->lentidao = LENTIDAO_BASE;
	}
}

void AttackClosePlayers(Jogador * p) {
	for (int i = (p->y - 1); i <= (p->y + 1); i++)
	{
		for (int j = (p->x - 1); j <= (p->x + 1); j++)
		{
			if (i == p->y && j == p->x) continue;
			if (hasPlayerIn(j, i) && gClients[gLabirinto.labirinto[i][j]].hp > 0) {
				gClients[gLabirinto.labirinto[i][j]].hp -= (UseStone(p) ? 2 : 1);
				p->atkCounter = (int)LENTIDAO_BASE;
				return;
			}
		}
	}
}

BOOL UseStone(Jogador * p) {
	if (p->stoneAutoHit == TRUE && p->nStones > 0) {
		--p->nStones;
		return TRUE;
	}
	return FALSE;
}

void DropStones(Jogador * p) {
	int cellValue = (p->nStones > 0) ? (p->nStones * PEDRAS) : EMPTY;
	gLabirinto.labirinto[p->y][p->x] = cellValue;
}

void AskPlayerToCollectItems(Jogador * p) {
	int nPedras = 0;
	/*
	* Item que está nas cordenadas actuais do jogador,
	* mas que o movimento ainda não tenha sido registado na matriz global
	* para ele poder obter o objecto.
	* Depois de apanhado, a posição do objecto na matriz,
	* deve de ser sobreposta com o id do utilizador.
	*/
	switch (gLabirinto.labirinto[p->x][p->y])
	{
	case VITAMINA:
		if ((p->hp + 1) <= (int)HP_BASE * 2) p->hp++;
		break;
	case ORANGE_BULL:
		if ((p->hp + 1) <= (int)HP_BASE * 2) p->hp += 3;
		break;
	case REB_CAFEINA:
		p->lentidao = (int)LENTIDAO_BASE - 2; //Efeito só deve de durar por 1 min (does not stack)
		p->itemDurationCounter = 15 * 60; // 15 instantes por segundo * 60 segundos = 900 instantes;
		break;
	default:
		if ((nPedras = gLabirinto.labirinto[p->x][p->y]) > PEDRAS){
			nPedras -= (int)PEDRAS;
			if (p->nStones + nPedras <= (int)PLAYER_STONE_CAP) p->nStones += nPedras;
		}
		else {
			_tprintf(TEXT("[SERVER] Falha ao apanhar objecto!"));
		}
		break;
	}
}