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