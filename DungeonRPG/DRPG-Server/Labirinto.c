#include "Labirinto.h"

Labirinto NovoLabirinto() {
	Labirinto l;
	for (int i = 0; i < 70; i++)
	{
		for (int j = 0; j < 70; j++)
		{
			if (i == 0 || i == 69 || j == 0 || j == 69)
			{
				l.labirinto[i][j] = STONEWALL;
			}
			else
			{
				l.labirinto[i][j] = EMPTY;
			}
		}
	}
	l.init = FALSE;
	return l;
}

void posicionaJogador(Jogador *j, Labirinto *l)
{
	j->x = 10;
	j->y = 10;
	l->labirinto[j->x][j->y] = 2;
}