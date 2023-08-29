#include "GameBoard.h"

GameBoard NovoGameBoard() {
	GameBoard l;
	for (int i = 0; i < 70; i++)
	{
		for (int j = 0; j < 70; j++)
		{
			if (i == 0 || i == 69 || j == 0 || j == 69)
			{
				l.gameBoard[i][j] = STONEWALL;
			}
			else
			{
				l.gameBoard[i][j] = EMPTY;
			}
		}
	}
	l.init = FALSE;
	return l;
}

GameBoard LerGameBoard() {
	GameBoard l;
	FILE *f;
	int n = 0, i = 0, j = 0;

	if ((f = fopen("jogo.txt", "r")) == NULL) {
		_tprintf(TEXT("[ERRO] Ao ler o ficheiro!"));
		system("pause");
		exit(-1);
	}

	while (fscanf(f, "%d,", &n) != EOF)
	{
		l.gameBoard[i][j] = n;
		if (j == GAMEBOARDSIZE - 1)
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
	return shGameBoard->gameBoard[y][x] >= PLAYER_START_INDEX
		&& shGameBoard->gameBoard[y][x] <= PLAYER_END_INDEX;
}

BOOL hasMonsterAndPlayerIn(int x, int y) {
	return shGameBoard->gameBoard[y][x] > 1000;
}

BOOL hasObjectIn(int x, int y) {
	return (shGameBoard->gameBoard[y][x] >= ITEM_START_INDEX
		&& shGameBoard->gameBoard[y][x] <= ITEM_END_INDEX)
		|| (shGameBoard->gameBoard[y][x] > PEDRAS //Ver common.h
			&& shGameBoard->gameBoard[y][x] < 600);
}

BOOL hasWallIn(int x, int y) {
	return shGameBoard->gameBoard[y][x] >= WALL_START_INDEX
		&& shGameBoard->gameBoard[y][x] <= WALL_END_INDEX;
}

void DistribuirItems() {
	int x, y;
	srand(time(NULL));

	//Distribuir vitaminas
	for (int i = 0; i < ((GAMEBOARDSIZE * GAMEBOARDSIZE) * QNT_VITAMINA); i++)
	{
		//Obter coordenadas enquanto não encontrar uma posição vazia
		do {
			x = (rand() % GAMEBOARDSIZE);
			y = (rand() % GAMEBOARDSIZE);
		} while (!(shGameBoard->gameBoard[y][x] == EMPTY));
		shGameBoard->gameBoard[y][x] = VITAMINA;
	}

	//Distribuir Orange Bull
	for (int i = 0; i < ((GAMEBOARDSIZE * GAMEBOARDSIZE) * QNT_ORANGE_BULL); i++)
	{
		//Obter coordenadas enquanto não encontrar uma posição vazia
		do {
			x = (rand() % GAMEBOARDSIZE);
			y = (rand() % GAMEBOARDSIZE);
		} while (!(shGameBoard->gameBoard[y][x] == EMPTY));
		shGameBoard->gameBoard[y][x] = ORANGE_BULL;
	}
	
	//Distribuir Rebuçados de Cafeina
	for (int i = 0; i < ((GAMEBOARDSIZE * GAMEBOARDSIZE) * QNT_REB_CAFEINA); i++)
	{
		//Obter coordenadas enquanto não encontrar uma posição vazia
		do {
			x = (rand() % GAMEBOARDSIZE);
			y = (rand() % GAMEBOARDSIZE);
		} while (!(shGameBoard->gameBoard[y][x] == EMPTY));
		shGameBoard->gameBoard[y][x] = REB_CAFEINA;
	}

	//Distribuir Pedras
	for (int i = 0; i < ((GAMEBOARDSIZE * GAMEBOARDSIZE) * QNT_PEDRAS); i++)
	{
		//Obter coordenadas enquanto não encontrar uma posição vazia
		do {
			x = (rand() % GAMEBOARDSIZE);
			y = (rand() % GAMEBOARDSIZE);
		} while (!(shGameBoard->gameBoard[y][x] == EMPTY));
		shGameBoard->gameBoard[y][x] = (int)PEDRAS + 1;
	}
}