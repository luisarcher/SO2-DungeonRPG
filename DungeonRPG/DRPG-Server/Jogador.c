#include "Jogador.h"
#include "Labirinto.h"
#include "Common.h"
#include "Server.h"


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
			if (j->y > 1)
			{
				gLabirinto.labirinto[j->x][j->y] = EMPTY;
				j->y--;
				gLabirinto.labirinto[j->x][j->y] = playerId;
			}
			break;
		}
		case KEY_DOWN:
		{
			if (j->y < LABIRINTOSIZE-1)
			{
				gLabirinto.labirinto[j->x][j->y] = EMPTY;
				j->y++;
				gLabirinto.labirinto[j->x][j->y] = playerId;
			}
			break;
		}
		case KEY_LEFT:
		{
			if (j->x > 1)
			{
				gLabirinto.labirinto[j->x][j->y] = EMPTY;
				j->x--;
				gLabirinto.labirinto[j->x][j->y] = playerId;
			}
			break;
		}
		case KEY_RIGHT:
		{
			if (j->x < LABIRINTOSIZE - 1)
			{
				gLabirinto.labirinto[j->x][j->y] = EMPTY;
				j->x++;
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
	_tprintf(TEXT("POSX: %d POSY: %d\n\n"), x, y);
	
	/*int offset = (int)PLAYER_LOS / 2;
	//validar o scroll
	// - Definir margens e encostar o scroll ao mapa
	if ((x - offset) < 0) x = offset;
	else if ((x + offset) > LABIRINTOSIZE - 1) x = LABIRINTOSIZE - 1 - offset;
	if ((y - offset) < 0) y = offset;
	else if ((y + offset) > LABIRINTOSIZE - 1) y = LABIRINTOSIZE - 1 - offset;
	//The Matrix is a system, Neo. That system is our enemy.
	int _i = 0;
	int _j = 0;
	for (int i = y - offset; i <= y + offset; i++)
	{
		
		for (int j = x - offset; j <= x + offset; j++)
		{
			
			matriz[_i][_j] = gLabirinto.labirinto[i][j];
			_j++;
		}
		_i++; 
		_j = 0;
	}*/
	
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
			matriz[m][n] = gLabirinto.labirinto[j][i];
			m++;
			
		}
		n++;
		m = 0;
	}
	for (int i = 0; i < 10; i++)
	{

		for (int j = 0; j < 10; j++)
		{
			_tprintf(TEXT("%d"), matriz[j][i]);
		}
		_tprintf(TEXT("\n"));
	}
	
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