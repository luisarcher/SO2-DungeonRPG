#include "Jogador.h"

Jogador NovoJogador(TCHAR *n) {
	Jogador j;
	_tcscpy(j.nome, n);
	j.x = 0;
	j.y = 0;
	j.hp = 100;
	j.id = 0;
	return  j;
}

int MoverJogador(int playerId) {
	int c = 0;
	while (1) {//função de movimento a passar pra fora
		c = getch();
		if (c == 0 || c == 224)
		{
			switch ((c = getch())) {
			case KEY_UP:
			{
				if (j.x > 1)
				{
					l.labirinto[j.x][j.y] = 0;
					j.x--;
					l.labirinto[j.x][j.y] = 2;
					desenhaLabirinto(l);
				}
				/*else
				desenhaLabirinto(l);*/
				break;
			}
			case KEY_DOWN:
			{
				if (j.x < 68)
				{
					l.labirinto[j.x][j.y] = 0;
					j.x++;
					l.labirinto[j.x][j.y] = 2;
					desenhaLabirinto(l);
				}
				/*else
				desenhaLabirinto(l);*/
				break;
			}
			case KEY_LEFT:
			{
				if (j.y > 1)
				{
					l.labirinto[j.x][j.y] = 0;
					j.y--;
					l.labirinto[j.x][j.y] = 2;
					//desenhaLabirinto(l);
				}
				/*else
				desenhaLabirinto(l);*/
				break;
			}
			case KEY_RIGHT:
			{
				if (j.y < 68)
				{
					l.labirinto[j.x][j.y] = 0;
					j.y++;
					l.labirinto[j.x][j.y] = 2;
					//desenhaLabirinto(l);
				}
				/*else
				desenhaLabirinto(l);*/
				break;
			}
			default:
			{
				//desenhaLabirinto(l);

				break;
			}
			}
		}
	}

	return 0;
}