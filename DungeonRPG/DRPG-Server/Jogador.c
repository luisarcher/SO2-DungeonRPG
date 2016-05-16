#include "Jogador.h"
#include "Labirinto.h"

void NovoJogador(Jogador *j) {
	_tcscpy(j->nome, "guest");
	j->x = 0;
	j->y = 0;
	j->hp = 100;
	j->id = 0;
	j->nStones = 0;
	j->stoneAutoHit = TRUE;
}

int MoverJogador(int playerId, int keystroke) {
	Jogador *j = &gClients[playerId];
	//if(gLabirinto.labirinto[j.x][j.y] > 1000)
	//existe um monstro na mesma posição que o jogador

	switch (keystroke) {
		case KEY_UP:
		{
			if (j->x > 1)
			{
				gLabirinto.labirinto[j->x][j->y] = EMPTY;
				j->x--;
				gLabirinto.labirinto[j->x][j->y] = playerId;
			}
			break;
		}
		case KEY_DOWN:
		{
			if (j->x < 68)
			{
				gLabirinto.labirinto[j->x][j->y] = EMPTY;
				j->x++;
				gLabirinto.labirinto[j->x][j->y] = playerId;
			}
			break;
		}
		case KEY_LEFT:
		{
			if (j->y > 1)
			{
				gLabirinto.labirinto[j->x][j->y] = EMPTY;
				j->y--;
				gLabirinto.labirinto[j->x][j->y] = playerId;
			}
			break;
		}
		case KEY_RIGHT:
		{
			if (j->y < 68)
			{
				gLabirinto.labirinto[j->x][j->y] = EMPTY;
				j->y++;
				gLabirinto.labirinto[j->x][j->y] = playerId;
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

void UpdatePlayerLOS(int x, int y, int(*matriz)[PLAYER_LOS], int id) {
	//validar o scroll
	// - Definir margens e encostar o scroll ao mapa
	if ((x - PLAYER_LOS) < 0) x = PLAYER_LOS;
	else if ((x + PLAYER_LOS) > LABIRINTOSIZE - 1) x = LABIRINTOSIZE - 1 - PLAYER_LOS;
	if ((y - PLAYER_LOS) < 0) y = PLAYER_LOS;
	else if ((y + PLAYER_LOS) > LABIRINTOSIZE - 1) y = LABIRINTOSIZE - 1 - PLAYER_LOS;

	//The Matrix is a system, Neo. That system is our enemy.
	int _i = 0;
	int _j = 0;
	for (int i = y - PLAYER_LOS; i <= y + PLAYER_LOS; i++)
	{
		++_i; _j = 0;
		for (int j = x - PLAYER_LOS; j <= x + PLAYER_LOS; j++)
		{
			_j++;
			matriz[_i][_j] = gLabirinto.labirinto[i][j];
		}
	}
	


	matriz[PLAYER_LOS/2][PLAYER_LOS/2] = id;
}

void SetEmptyMatrix(int(*matriz)[PLAYER_LOS]) {
	for (int i = 0; i < PLAYER_LOS; i++)
	{
		for (int j = 0; j < PLAYER_LOS; j++)
		{
			matriz[i][j] = EMPTY;
		}
	}
}