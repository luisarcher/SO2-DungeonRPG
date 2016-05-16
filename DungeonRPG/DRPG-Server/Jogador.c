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
				j->y--;
				gLabirinto.labirinto[j->y][j->x] = playerId;
			}
			break;
		}
		case KEY_DOWN:
		{
			if (j->y < LABIRINTOSIZE-2)
			{
				gLabirinto.labirinto[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (gLabirinto.labirinto[j->y][j->x] / 100) : EMPTY);
				j->y++;
				gLabirinto.labirinto[j->y][j->x] = playerId;
			}
			break;
		}
		case KEY_LEFT:
		{
			if (j->x > 1)
			{
				gLabirinto.labirinto[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (gLabirinto.labirinto[j->y][j->x] / 100) : EMPTY);
				j->x--;
				gLabirinto.labirinto[j->y][j->x] = playerId;
			}
			break;
		}
		case KEY_RIGHT:
		{
			if (j->x < LABIRINTOSIZE - 2)
			{
				gLabirinto.labirinto[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (gLabirinto.labirinto[j->y][j->x] / 100) : EMPTY);
				j->x++;
				gLabirinto.labirinto[j->y][j->x] = playerId;
			}
			break;
		}
		default:
		{
			//Nothing to do here
			break;
		}
	}
	return 0;
}

void UpdatePlayerLOS(int x, int y, int(*matriz)[PLAYER_LOS]) {
	//validar o scroll
	// - Definir margens e encostar o scroll ao mapa
	SetEmptyMatrix(matriz);
	//_tprintf(TEXT("POSX: %d POSY: %d\n\n"), x, y);
	
	int iniX, iniY, maxX,maxY;
	if (x - 5 < 0)
	{
		iniX = 0;
	}
	else
		iniX = x - 5;
	if (x + 5 > LABIRINTOSIZE)
	{
		maxX = LABIRINTOSIZE;
	}
	else
		maxX = x + 5;
	if (y - 5 < 0)
	{
		iniY = 0;
	}
	else
		iniY = y - 5;
	if (y + 5 > LABIRINTOSIZE)
	{
		maxY = LABIRINTOSIZE;
	}
	else
		maxY = y + 5;
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
	/*for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			_tprintf(TEXT("%d"), matriz[j][i]);
		}
		_tprintf(TEXT("\n"));
	}*/
}

void SetEmptyMatrix(int(*matriz)[PLAYER_LOS]) {
	for (int i = 0; i < PLAYER_LOS; i++)
	{
		for (int j = 0; j < PLAYER_LOS; j++)
		{
			matriz[i][j] = FOG_OF_WAR;
		}
	}
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

//Collect items em mover
//verificar se >1000 (monstros)
//ler git