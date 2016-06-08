#pragma once
#include "Monster.h"

Monstro NovoMonstroBully(int nPassos) {
	Monstro m;
	m.hp = HP_BASE;
	m.lentidao = 7;
	m.tipo = 51;
	m.posX = 0;
	m.posY = 0;
	m.stamina = m.lentidao;
	m.passos = nPassos;
	m.direcao = 2;
	return m;
}

Monstro NovoMonstroDistraido(int nPassos) {
	Monstro m;
	m.hp = HP_BASE;
	m.lentidao = 3;
	m.tipo = 51;
	m.posX = 0;
	m.posY = 0;
	m.passos = nPassos;
	m.stamina = m.lentidao;
	return m;
}

void MoveMonstro(Labirinto * shLab, int d, Monstro *m) {
	switch (d)
	{
	case UP:
	{		
		//Labirinto Ocupado - Bloqueia o acesso ao labirinto por outras threads
		WaitForSingleObject(gMutexLabirinto, INFINITE);

		if ((m->posY - 1) >= 1 && !(shLab->labirinto[m->posY-1][m->posX] >= WALL_START_INDEX && shLab->labirinto[m->posY-1][m->posX] <= WALL_END_INDEX)) {
			if (shLab->labirinto[m->posY][m->posX] > 1000)
			{
				shLab->labirinto[m->posY][m->posX] = shLab->labirinto[m->posY][m->posX] - (m->tipo * 100);
			}
			else
			{
				shLab->labirinto[m->posY][m->posX] = EMPTY;
			}
			m->posY = m->posY - 1;
			if (shLab->labirinto[m->posY][m->posX] == EMPTY)
			{
				shLab->labirinto[m->posY][m->posX] = m->tipo;
			}
			else {
				shLab->labirinto[m->posY][m->posX] = shLab->labirinto[m->posY][m->posX] + (m->tipo * 100);
			}
			//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
			ReleaseMutex(gMutexLabirinto);
			break;
		}
		else {
			m->direcao = MudaDirecao(UP);

			//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
			//A função pai, tem de entregar o recurso antes do "filho" pedir.
			ReleaseMutex(gMutexLabirinto);
			MoveMonstro(shLab, m->direcao, m);
			break;
		}
		
	}
	case DOWN:
	{	
		//Labirinto Ocupado - Bloqueia o acesso ao labirinto por outras threads
		WaitForSingleObject(gMutexLabirinto, INFINITE);
		if ((m->posY + 1) <= LABIRINTOSIZE - 2 && !(shLab->labirinto[m->posY+1][m->posX] >= WALL_START_INDEX && shLab->labirinto[m->posY+1][m->posX] <= WALL_END_INDEX)) {
			if (shLab->labirinto[m->posY][m->posX] > 1000)
			{
				shLab->labirinto[m->posY][m->posX] = shLab->labirinto[m->posY][m->posX] - (m->tipo * 100);
			}
			else
			{
				shLab->labirinto[m->posY][m->posX] = EMPTY;
			}
			m->posY = m->posY + 1;
			if (shLab->labirinto[m->posY][m->posX] == EMPTY)
			{
				shLab->labirinto[m->posY][m->posX] = m->tipo;
			}
			else {
				shLab->labirinto[m->posY][m->posX] = shLab->labirinto[m->posY][m->posX] +( m->tipo * 100);
			}
			//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
			ReleaseMutex(gMutexLabirinto);
			break;
		}
		else {
			m->direcao = MudaDirecao(DOWN);
			//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
			ReleaseMutex(gMutexLabirinto);
			MoveMonstro(shLab, m->direcao, m);
			break;
		}
		
	}
	case RIGHT:
	{	
		//Labirinto Ocupado - Bloqueia o acesso ao labirinto por outras threads
		WaitForSingleObject(gMutexLabirinto, INFINITE);

		if ((m->posX + 1) <= LABIRINTOSIZE - 2 && !(shLab->labirinto[m->posY][m->posX+1] >= WALL_START_INDEX && shLab->labirinto[m->posY][m->posX+1] <= WALL_END_INDEX)) {
			if (shLab->labirinto[m->posY][m->posX] > 1000)
			{
				shLab->labirinto[m->posY][m->posX] = shLab->labirinto[m->posY][m->posX] - (m->tipo * 100);
			}
			else
			{
				shLab->labirinto[m->posY][m->posX] = EMPTY;
			}
			m->posX = m->posX + 1;
			if (shLab->labirinto[m->posY][m->posX] == EMPTY)
			{
				shLab->labirinto[m->posY][m->posX] = m->tipo;
			}
			else {
				shLab->labirinto[m->posY][m->posX] = shLab->labirinto[m->posY][m->posX] + (m->tipo * 100);
			}
			//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
			ReleaseMutex(gMutexLabirinto);
			break;
		}
		else {
			m->direcao = MudaDirecao(RIGHT);
			//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
			ReleaseMutex(gMutexLabirinto);
			MoveMonstro(shLab, m->direcao, m);
			break;
		}
		
	}
	case LEFT:
	{	
		//Labirinto Ocupado - Bloqueia o acesso ao labirinto por outras threads
		WaitForSingleObject(gMutexLabirinto, INFINITE);

		if ((m->posX - 1) >= 1 && !(shLab->labirinto[m->posY][m->posX - 1] >= WALL_START_INDEX && shLab->labirinto[m->posY][m->posX - 1] <= WALL_END_INDEX)) {
			if (shLab->labirinto[m->posY][m->posX] > 1000)
			{
				shLab->labirinto[m->posY][m->posX] = shLab->labirinto[m->posY][m->posX] - (m->tipo * 100);
			}
			else
			{
				shLab->labirinto[m->posY][m->posX] = EMPTY;
			}
			m->posX = m->posX - 1;
			if (shLab->labirinto[m->posY][m->posX] == EMPTY)
			{
				shLab->labirinto[m->posY][m->posX] = m->tipo;
			}
			else {
				shLab->labirinto[m->posY][m->posX] = shLab->labirinto[m->posY][m->posX] + (m->tipo * 100);
			}
			//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
			ReleaseMutex(gMutexLabirinto);
			break;
		}
		else {
			m->direcao = MudaDirecao(LEFT);
			//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
			ReleaseMutex(gMutexLabirinto);
			MoveMonstro(shLab, m->direcao, m);
			break;
		}
		
	}
	default:
		break;
	}
	//No fim de mover, sinaliza o evento que vai actualizar a interface dos clientes.
	SetEvent(ghUpdateGameClientEvent);
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
		_tprintf(TEXT("[Erro] Criar objectos mapeados(%d)\n"), GetLastError());
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

void DisplayMonsterSurroundings(int x, int y) {
	int matriz[15][15]; //Criar um diâmentro de visão para o monstro

	SetEmptyMatrix(&matriz);

	int iniX, iniY, maxX, maxY;

	if (x - 5 < 0) iniX = 0;
	else iniX = x - 5;
	if (x + 5 > LABIRINTOSIZE) maxX = LABIRINTOSIZE;
	else maxX = x + 5;

	if (y - 5 < 0) iniY = 0;
	else iniY = y - 5;
	if (y + 5 > LABIRINTOSIZE) maxY = LABIRINTOSIZE;
	else maxY = y + 5;

	int m = 0, n = 0;

	//Labirinto Ocupado - Bloqueia o acesso ao labirinto por outras threads
	WaitForSingleObject(gMutexLabirinto, INFINITE);

	for (int i = iniY; i < maxY; i++, n++, m = 0)
	{
		for (int j = iniX; j < maxX; j++, m++)
		{
			matriz[n][m] = shLabirinto->labirinto[i][j];
		}
	}

	//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
	ReleaseMutex(gMutexLabirinto);
}

void ReadSharedMemory() {
	//while (!fim) {
		system("cls");
		for (int i = 0; i < LABIRINTOSIZE; i++)
		{
			for (int j = 0; j < LABIRINTOSIZE; j++)
			{
				int c = shLabirinto->labirinto[i][j];
				gotoxy(j, i);
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
	CloseHandle(*hMappedObj);
}


void gotoxy(int x, int y) {
	COORD coord;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(hStdout, coord);
}


void escondeCursor() {
	CONSOLE_CURSOR_INFO cci;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	cci.bVisible = FALSE;				// Desativa a visibilidade do cursor [TRUE/FALSE]
	cci.dwSize = 20;					// Define a altura do cursor [1-100]

	SetConsoleCursorInfo(hStdout, &cci);	// Aplica as definições anteriores
}

void CheckForThreats(Monstro *m) {
	if (shLabirinto->labirinto[m->posY][m->posX] > 1000)
		m->hp++;
}