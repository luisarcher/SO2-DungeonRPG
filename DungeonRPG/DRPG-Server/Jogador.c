#include "Jogador.h"
#include "Labirinto.h"
#include "Common.h"
#include "Server.h"

void NovoJogador(Jogador *j, int id) {
	_tcscpy(j->nome, "guest");
	j->hp = HP_BASE;
	j->lentidao = LENTIDAO_BASE;
	j->id = id;
	j->nStones = 0;
	j->stoneAutoHit = TRUE;

	//Define x e y do jogador (pos vazia) e regista-o no labirinto
	SetPlayerInRandomPosition(j);
}

int MoverJogador(int playerId, int keystroke) {
	Jogador *j = &gClients[playerId];
	//if(gLabirinto.labirinto[j.x][j.y] > 1000)
	//existe um monstro na mesma posição que o jogador

	switch (keystroke) {
		case KEY_UP:
		{
			if (j->y > 1)
			{
				gLabirinto.labirinto[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (gLabirinto.labirinto[j->y][j->x] / 100) : EMPTY);
				if (!hasWallIn(j->x, j->y - 1) && !hasPlayerIn(j->x, j->y - 1)) j->y--;
			}
			break;
		}
		case KEY_DOWN:
		{
			if (j->y < LABIRINTOSIZE-2)
			{
				gLabirinto.labirinto[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (gLabirinto.labirinto[j->y][j->x] / 100) : EMPTY);
				if (!hasWallIn(j->x, j->y + 1) && !hasPlayerIn(j->x, j->y + 1))j->y++;
			}
			break;
		}
		case KEY_LEFT:
		{
			if (j->x > 1)
			{
				gLabirinto.labirinto[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (gLabirinto.labirinto[j->y][j->x] / 100) : EMPTY);
				if (!hasWallIn(j->x - 1, j->y) && !hasPlayerIn(j->x - 1, j->y)) j->y--; j->x--;
			}
			break;
		}
		case KEY_RIGHT:
		{
			if (j->x < LABIRINTOSIZE - 2)
			{
				gLabirinto.labirinto[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (gLabirinto.labirinto[j->y][j->x] / 100) : EMPTY);
				if (!hasWallIn(j->x + 1, j->y) && !hasPlayerIn(j->x + 1, j->y)) j->x++;
			}
			break;
		}
		default:
		{
			_tprintf(TEXT("[SERVER] Não foi possível movimentar o jogador!"));
			break;
		}
		if (hasObjectIn(j->x, j->y)) AskPlayerToCollectItems(j);
		//Update matrix after collected items
		gLabirinto.labirinto[j->y][j->x] = playerId;
	}
	return 0;
}

void UpdatePlayerLOS(int x, int y, int(*matriz)[PLAYER_LOS]) {
	//validar o scroll
	// - Definir margens e encostar o scroll ao mapa
	SetEmptyMatrix(matriz);
	//_tprintf(TEXT("POSX: %d POSY: %d\n\n"), x, y);
	
	int iniX, iniY, maxX,maxY;

	if (x - 5 < 0) iniX = 0;
	else iniX = x - 5;
	if (x + 5 > LABIRINTOSIZE) maxX = LABIRINTOSIZE;
	else maxX = x + 5;

	if (y - 5 < 0) iniY = 0;
	else iniY = y - 5;
	if (y + 5 > LABIRINTOSIZE) maxY = LABIRINTOSIZE;
	else maxY = y + 5;

	int m=0, n= 0;
	for (int i = iniY; i < maxY; i++)
	{
		for (int j = iniX; j < maxX; j++)
		{
			matriz[m][n] = gLabirinto.labirinto[i][j];
			m++;
		}
		n++;
		m = 0;
	}
}

void SetEmptyMatrix(int(*matriz)[PLAYER_LOS]) {
	for (int i = 0; i < PLAYER_LOS; i++)
		for (int j = 0; j < PLAYER_LOS; j++)
			matriz[i][j] = FOG_OF_WAR;
}

void SetPlayerInRandomPosition(Jogador * p) {
	int x, y;
	do {
		srand(time(NULL));
		x = (rand() % LABIRINTOSIZE);
		srand(time(NULL));
		y = (rand() % LABIRINTOSIZE);
	} while (!gLabirinto.labirinto[y][x] == EMPTY);

	p->x = x;
	p->y = y;
	gLabirinto.labirinto[y][x] = p->id;
}

BOOL hasPlayerIn(int x, int y) {
	return gLabirinto.labirinto[y][x] >= PLAYER_START_INDEX
		&& gLabirinto.labirinto[y][x] <= PLAYER_END_INDEX;
}

BOOL hasMonsterAndPlayerIn(int x, int y) {
	return gLabirinto.labirinto[y][x] > 1000;
}

BOOL hasObjectIn(int x, int y) {
	return (gLabirinto.labirinto[y][x] >= ITEM_START_INDEX
		&& gLabirinto.labirinto[y][x] <= ITEM_END_INDEX)
		|| (gLabirinto.labirinto[y][x] > PEDRAS //Ver common.h
		&& gLabirinto.labirinto[y][x] < 600);
}

BOOL hasWallIn(int x, int y) {
	return gLabirinto.labirinto[y][x] >= WALL_START_INDEX
		&& gLabirinto.labirinto[y][x] <= WALL_END_INDEX;
}

void AskPlayerToCollectItems(Jogador * p) {
	int nPedras = 0;
	/*
	* Item que está nas cordenadas actuais do jogador,
	* mas que o movimento ainda não tenha sido registado na matriz global
	* para ele poder obter o objecto.
	* Depois de apanhado, a posição do objecto na matriz,
	* é sobreposta com o id do utilizador.
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
		p->lentidao -= 2; //Efeito só deve de durar por 1 min (does not stack)
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