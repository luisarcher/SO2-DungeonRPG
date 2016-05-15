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
	Jogador j = gClients[playerId];
	//if(gLabirinto.labirinto[j.x][j.y] > 1000)
	//existe um monstro na mesma posi��o que o jogador

	switch (keystroke) {
		case KEY_UP:
		{
			if (j.x > 1)
			{
				gLabirinto.labirinto[j.x][j.y] = EMPTY;
				j.x--;
				gLabirinto.labirinto[j.x][j.y] = playerId;
			}
			break;
		}
		case KEY_DOWN:
		{
			if (j.x < 68)
			{
				gLabirinto.labirinto[j.x][j.y] = EMPTY;
				j.x++;
				gLabirinto.labirinto[j.x][j.y] = playerId;
			}
			break;
		}
		case KEY_LEFT:
		{
			if (j.y > 1)
			{
				gLabirinto.labirinto[j.x][j.y] = EMPTY;
				j.y--;
				gLabirinto.labirinto[j.x][j.y] = playerId;
			}
			break;
		}
		case KEY_RIGHT:
		{
			if (j.y < 68)
			{
				gLabirinto.labirinto[j.x][j.y] = EMPTY;
				j.y++;
				gLabirinto.labirinto[j.x][j.y] = playerId;
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

void PlayerLOS(int * matrix) {
	//Obter o que est� � volta do jogador para enviar ao cliente
	int m[10][10];

}