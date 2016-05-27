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

Labirinto LerLabirinto() {
	Labirinto l;
	FILE *f;
	int n = 0, i = 0, j = 0;

	if ((f = fopen("jogo.txt", "r")) == NULL) {
		_tprintf(TEXT("[ERRO] Ao ler o ficheiro!"));
		system("pause");
		exit(-1);
	}

	while (fscanf(f, "%d,", &n) != EOF)
	{
		l.labirinto[i][j] = n;
		if (j == LABIRINTOSIZE - 1)
		{
			j = 0;
			i++;
		}
		else
		{
			j++;
		}
	}
	fclose(f);
	return l;
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

BOOL hasStamina(Jogador p) {
	return (p.lentidaoCounter == 0);
}

void DistribuirItems() {
	int x, y;
	srand(time(NULL));

	//Distribuir vitaminas
	for (int i = 0; i < QNT_VITAMINA; i++)
	{
		//Obter coordenadas enquanto não encontrar uma posição vazia
		do {
			x = (rand() % LABIRINTOSIZE);
			y = (rand() % LABIRINTOSIZE);
		} while (!gLabirinto.labirinto[y][x] == EMPTY);
		gLabirinto.labirinto[y][x] = VITAMINA;
	}

	//Distribuir Orange Bull
	for (int i = 0; i < QNT_ORANGE_BULL; i++)
	{
		//Obter coordenadas enquanto não encontrar uma posição vazia
		do {
			x = (rand() % LABIRINTOSIZE);
			y = (rand() % LABIRINTOSIZE);
		} while (!gLabirinto.labirinto[y][x] == EMPTY);
		gLabirinto.labirinto[y][x] = ORANGE_BULL;
	}
	
	//Distribuir Rebuçados de Cafeina
	for (int i = 0; i < QNT_REB_CAFEINA; i++)
	{
		//Obter coordenadas enquanto não encontrar uma posição vazia
		do {
			x = (rand() % LABIRINTOSIZE);
			y = (rand() % LABIRINTOSIZE);
		} while (!gLabirinto.labirinto[y][x] == EMPTY);
		gLabirinto.labirinto[y][x] = REB_CAFEINA;
	}

	//Distribuir Pedras
	for (int i = 0; i < QNT_PEDRAS; i++)
	{
		//Obter coordenadas enquanto não encontrar uma posição vazia
		do {
			x = (rand() % LABIRINTOSIZE);
			y = (rand() % LABIRINTOSIZE);
		} while (!gLabirinto.labirinto[y][x] == EMPTY);
		gLabirinto.labirinto[y][x] = QNT_PEDRAS;
	}
}