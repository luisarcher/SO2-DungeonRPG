#pragma once
#include "Monster.h"


//Monstro

Monstro NovoMonstroBully() {
	Monstro *m = malloc(sizeof(Monstro));
	m->hp = 10;
	m->lentidao = 7;
	m->tipo = 51;
	m->posX = 0;
	m->posY = 0;
	//srand(time(NULL));

	m->direcao = 2;
	return *m;
}

Monstro NovoMonstroDistraido() {
	Monstro *m = malloc(sizeof(Monstro));
	m->hp = 10;
	m->lentidao = 3;
	m->tipo = 51;
	m->posX = 0;
	m->posY = 0;
	return *m;
}

void MoveMonstro(Labirinto * shLab, int d, Monstro *m) {
	switch (d)
	{
	case UP:
	{	
		
		if ((m->posY - 1) >= 1 && !(shLab->labirinto[m->posY-1][m->posX] >= WALL_START_INDEX && shLab->labirinto[m->posY-1][m->posX] <= WALL_END_INDEX)) {
			shLab->labirinto[m->posY][m->posX] = EMPTY;
			m->posY = m->posY - 1;
			shLab->labirinto[m->posY][m->posX] = m->tipo;
			break;
		}
		else {
			m->direcao = MudaDirecao(UP);
			MoveMonstro(shLab, m->direcao, m);
			break;
		}
		
	}
	case DOWN:
	{	
		
		if ((m->posY + 1) <= LABIRINTOSIZE - 2 && !(shLab->labirinto[m->posY+1][m->posX] >= WALL_START_INDEX && shLab->labirinto[m->posY+1][m->posX] <= WALL_END_INDEX)) {
			shLab->labirinto[m->posY][m->posX] = EMPTY;
			m->posY = m->posY + 1;
			shLab->labirinto[m->posY][m->posX] = m->tipo;
			break;
		}
		else {
			m->direcao = MudaDirecao(DOWN);
			MoveMonstro(shLab, m->direcao, m);
			break;
		}
		
	}
	case RIGHT:
	{	
		if ((m->posX + 1) <= LABIRINTOSIZE - 2 && !(shLab->labirinto[m->posY][m->posX+1] >= WALL_START_INDEX && shLab->labirinto[m->posY][m->posX+1] <= WALL_END_INDEX)) {
			shLab->labirinto[m->posY][m->posX] = EMPTY;
			m->posX = m->posX + 1;
			shLab->labirinto[m->posY][m->posX] = m->tipo;
			break;
		}
		else {
			m->direcao = MudaDirecao(RIGHT);
			MoveMonstro(shLab, m->direcao, m);
			break;
		}
		
	}
	case LEFT:
	{	
		if ((m->posX - 1) >= 1 && !(shLab->labirinto[m->posY][m->posX - 1] >= WALL_START_INDEX && shLab->labirinto[m->posY][m->posX - 1] <= WALL_END_INDEX)) {
			shLab->labirinto[m->posY][m->posX] = EMPTY;
			m->posX = m->posX - 1;
			shLab->labirinto[m->posY][m->posX] = m->tipo;
			break;
		}
		else {
			m->direcao = MudaDirecao(LEFT);
			MoveMonstro(shLab, m->direcao, m);
			break;
		}
		
	}


	default:
		break;
	}
}

int MudaDirecao(int anterior) {
	int r = anterior;
	

	while (r == anterior)
	{
		r = rand() % 4 + 1;

	}
	return r;
}

//Memoria

void InitializeSharedMemory(HANDLE * hMappedObj) {

	*hMappedObj = OpenFileMapping(FILE_MAP_ALL_ACCESS,
		FALSE,
		TEXT("ShLabirinto"));
	if (hMappedObj == NULL) {
		_tprintf(TEXT("[Erro] Criar objectos mapeamentos(%d)\n"), GetLastError());
		system("pause");
		return -1;
	}

	shLabirinto = (Labirinto*)MapViewOfFile(*hMappedObj,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(Labirinto)
		);
	if (shLabirinto == NULL) {
		_tprintf(TEXT("[Erro] Mapear para memória(%d)\n"), GetLastError());
		system("pause");
		return -1;
	}
}

void ReadSharedMemory() {
	//while (!fim) {
		system("cls");
		for (int i = 0; i < LABIRINTOSIZE; i++)
		{
			for (int j = 0; j < LABIRINTOSIZE; j++)
			{
				int c = shLabirinto->labirinto[i][j];

				if (c >= WALL_START_INDEX && c <= WALL_END_INDEX)
				{
					_tprintf(TEXT("%c"), 9619);
				}
				else if (c == EMPTY)
				{
					_tprintf(TEXT("%c"), 9617);
				}
				else if (c >= PLAYER_START_INDEX && c <= PLAYER_END_INDEX)
				{
					_tprintf(TEXT("%c"), 64);
				}
				else if (c >= MONSTER_START_INDEX && c <= MONSTER_END_INDEX)
				{
					_tprintf(TEXT("%c"), 167);
				}
				else if (c >= ITEM_START_INDEX && c <= ITEM_END_INDEX)
				{
					_tprintf(TEXT("%c"), '+');
				}
				else if (c > PEDRAS && c < 600)
				{
					_tprintf(TEXT("%c"), 'P');
				}
			}
			_tprintf(TEXT("\n"));
		//}
		//Vai ler o que está em memória de 1 em 1 seg
		//Sleep(1000);
	}
}

void CloseHandles(HANDLE * hMappedObj) {
	UnmapViewOfFile(shLabirinto);
	CloseHandle(hMappedObj);
}