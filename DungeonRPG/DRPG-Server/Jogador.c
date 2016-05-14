#include "Jogador.h"
#include "Labirinto.h"

void NovoJogador(Jogador *j) {
	_tcscpy(j->nome, "guest");
	j->x = 0;
	j->y = 0;
	j->hp = 100;
	j->id = 0;
}

int MoverJogador(int playerId, int keystroke) {
	j = gClients[playerId];
	switch (keystroke) {
		case KEY_UP:
		{
			if (j.x > 1)
			{
				l.labirinto[j.x][j.y] = 0;
				j.x--;
				l.labirinto[j.x][j.y] = 2;
				//desenhaLabirinto(l);
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
				//desenhaLabirinto(l);
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
	return 0;
}

void PlayerLOS(int * matrix) {
	int m[10][10];

}