#pragma once
#include "Monster.h"

Monstro NovoMonstroBully(int nPassos,int hp,int x,int y) {
	Monstro m;
	m.hp = hp;
	m.lentidao = 7;
	m.tipo = 51;
	m.posX = x;
	m.posY = y;
	m.stamina = m.lentidao;
	m.passos = nPassos;
	m.direcao = 2;
	return m;
}

Monstro NovoMonstroDistraido(int nPassos,int hp,int x, int y) {
	Monstro m;
	m.hp = hp;
	m.lentidao = 3;
	m.tipo = 52;
	m.posX = x;
	m.posY = y;
	m.passos = nPassos;
	m.stamina = m.lentidao;
	return m;
}

void MoveMonstro(GameBoard * shLab, int d, Monstro *m) {
	switch (d)
	{
	case UP:
	{		
		//GameBoard Ocupado - Bloqueia o acesso ao gameBoard por outras threads
		WaitForSingleObject(gMutexGameBoard, INFINITE);

		if ((m->posY - 1) >= 1 && !hasWallIn(m->posX, m->posY - 1) && !hasMonsterIn(m->posX, m->posY - 1)) {
			if (shLab->gameBoard[m->posY][m->posX] > 1000)
			{
				shLab->gameBoard[m->posY][m->posX] = shLab->gameBoard[m->posY][m->posX] - (m->tipo * 100);
			}
			else
			{
				shLab->gameBoard[m->posY][m->posX] = EMPTY;
			}
			m->posY = m->posY - 1;
			if (shLab->gameBoard[m->posY][m->posX] == EMPTY)
			{
				shLab->gameBoard[m->posY][m->posX] = m->tipo;
			}
			else {
				shLab->gameBoard[m->posY][m->posX] = shLab->gameBoard[m->posY][m->posX] + (m->tipo * 100);
			}
			//Liberta GameBoard - Desbloqueia o acesso ao gameBoard a outras threads
			ReleaseMutex(gMutexGameBoard);
			break;
		}
		else {
			m->direcao = MudaDirecao(UP);

			//Liberta GameBoard - Desbloqueia o acesso ao gameBoard a outras threads
			//A função pai, tem de entregar o recurso antes do "filho" pedir.
			ReleaseMutex(gMutexGameBoard);
			MoveMonstro(shLab, m->direcao, m);
			break;
		}
		
	}
	case DOWN:
	{	
		//GameBoard Ocupado - Bloqueia o acesso ao gameBoard por outras threads
		WaitForSingleObject(gMutexGameBoard, INFINITE);
		if ((m->posY + 1) <= GAMEBOARDSIZE - 2 && !hasWallIn( m->posX, m->posY + 1 ) && !hasMonsterIn(m->posX, m->posY + 1)) {
			//_tprintf(TEXT("[sfsfsf] a mover para (%d)\n"), m->posY+1);
			//system("pause");
			if (shLab->gameBoard[m->posY][m->posX] > 1000)
			{
				shLab->gameBoard[m->posY][m->posX] = shLab->gameBoard[m->posY][m->posX] - (m->tipo * 100);
			}
			else
			{
				shLab->gameBoard[m->posY][m->posX] = EMPTY;
			}
			m->posY = m->posY + 1;
			if (shLab->gameBoard[m->posY][m->posX] == EMPTY)
			{
				shLab->gameBoard[m->posY][m->posX] = m->tipo;
			}
			else {
				shLab->gameBoard[m->posY][m->posX] = shLab->gameBoard[m->posY][m->posX] +( m->tipo * 100);
			}
			//Liberta GameBoard - Desbloqueia o acesso ao gameBoard a outras threads
			ReleaseMutex(gMutexGameBoard);
			break;
		}
		else {
			m->direcao = MudaDirecao(DOWN);
			//Liberta GameBoard - Desbloqueia o acesso ao gameBoard a outras threads
			ReleaseMutex(gMutexGameBoard);
			MoveMonstro(shLab, m->direcao, m);
			break;
		}
		
	}
	case RIGHT:
	{	
		//GameBoard Ocupado - Bloqueia o acesso ao gameBoard por outras threads
		WaitForSingleObject(gMutexGameBoard, INFINITE);

		if ((m->posX + 1) <= GAMEBOARDSIZE - 2 && !hasWallIn(m->posX + 1, m->posY) && !hasMonsterIn( m->posX+1, m->posY)) {
			if (shLab->gameBoard[m->posY][m->posX] > 1000)
			{
				shLab->gameBoard[m->posY][m->posX] = shLab->gameBoard[m->posY][m->posX] - (m->tipo * 100);
			}
			else
			{
				shLab->gameBoard[m->posY][m->posX] = EMPTY;
			}
			m->posX = m->posX + 1;
			if (shLab->gameBoard[m->posY][m->posX] == EMPTY)
			{
				shLab->gameBoard[m->posY][m->posX] = m->tipo;
			}
			else {
				shLab->gameBoard[m->posY][m->posX] = shLab->gameBoard[m->posY][m->posX] + (m->tipo * 100);
			}
			//Liberta GameBoard - Desbloqueia o acesso ao gameBoard a outras threads
			ReleaseMutex(gMutexGameBoard);
			break;
		}
		else {
			m->direcao = MudaDirecao(RIGHT);
			//Liberta GameBoard - Desbloqueia o acesso ao gameBoard a outras threads
			ReleaseMutex(gMutexGameBoard);
			MoveMonstro(shLab, m->direcao, m);
			break;
		}
		
	}
	case LEFT:
	{	
		//GameBoard Ocupado - Bloqueia o acesso ao gameBoard por outras threads
		WaitForSingleObject(gMutexGameBoard, INFINITE);

		if ((m->posX - 1) >= 1 && !hasWallIn(m->posX - 1, m->posY) && !hasMonsterIn(m->posX - 1, m->posY)) {
			if (shLab->gameBoard[m->posY][m->posX] > 1000)
			{
				shLab->gameBoard[m->posY][m->posX] = shLab->gameBoard[m->posY][m->posX] - (m->tipo * 100);
			}
			else
			{
				shLab->gameBoard[m->posY][m->posX] = EMPTY;
			}
			m->posX = m->posX - 1;
			if (shLab->gameBoard[m->posY][m->posX] == EMPTY)
			{
				shLab->gameBoard[m->posY][m->posX] = m->tipo;
			}
			else {
				shLab->gameBoard[m->posY][m->posX] = shLab->gameBoard[m->posY][m->posX] + (m->tipo * 100);
			}
			//Liberta GameBoard - Desbloqueia o acesso ao gameBoard a outras threads
			ReleaseMutex(gMutexGameBoard);
			break;
		}
		else {
			m->direcao = MudaDirecao(LEFT);
			//Liberta GameBoard - Desbloqueia o acesso ao gameBoard a outras threads
			ReleaseMutex(gMutexGameBoard);
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
		TEXT("ShGameBoard"));
	if (hMappedObj == NULL) {
		_tprintf(TEXT("[Erro] Criar objectos mapeados(%d)\n"), GetLastError());
		system("pause");
		return -1;
	}

	shGameBoard = (GameBoard*)MapViewOfFile(*hMappedObj,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(GameBoard)
		);
	if (shGameBoard == NULL) {
		_tprintf(TEXT("[Erro] Mapear para memória(%d)\n"), GetLastError());
		system("pause");
		return -1;
	}
}

void DisplayMonsterSurroundings(int x, int y,int hp) {
	//int matriz[MONSTER_MAT_SIZE][MONSTER_MAT_SIZE]; //Criar um diâmentro de visão para o monstro

	int iniX, iniY, maxX, maxY;

	if (x - (MONSTER_MAT_SIZE/2) < 0) iniX = 0;
	else iniX = x - MONSTER_MAT_SIZE / 2;
	if (x + MONSTER_MAT_SIZE / 2 > GAMEBOARDSIZE) maxX = GAMEBOARDSIZE;
	else maxX = x + MONSTER_MAT_SIZE / 2;

	if (y - MONSTER_MAT_SIZE / 2 < 0) iniY = 0;
	else iniY = y - MONSTER_MAT_SIZE / 2;
	if (y + MONSTER_MAT_SIZE / 2 > GAMEBOARDSIZE) maxY = GAMEBOARDSIZE;
	else maxY = y + MONSTER_MAT_SIZE / 2;

	int m = 0, n = 0;

	//GameBoard Ocupado - Bloqueia o acesso ao gameBoard por outras threads
	WaitForSingleObject(gMutexGameBoard, INFINITE);

	clrscr();
	for (int i = iniY; i < maxY; i++, n++, m = 0)
	{
		for (int j = iniX; j < maxX; j++, m++)
		{
			int c = shGameBoard->gameBoard[i][j];
			if (c > 1000) {
				c = c % 100;
				if (c >= PLAYER_START_INDEX && c <= PLAYER_END_INDEX)
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
			else
			{
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
			
		}
		_tprintf(TEXT("\n"));
	}
	_tprintf(TEXT("Monster at: Y:%d X:%d\n HP: %d"),y,x, hp);

	//Liberta GameBoard - Desbloqueia o acesso ao gameBoard a outras threads
	ReleaseMutex(gMutexGameBoard);
}

void ReadSharedMemory() {
	//while (!fim) {
		system("cls");
		for (int i = 0; i < GAMEBOARDSIZE; i++)
		{
			for (int j = 0; j < GAMEBOARDSIZE; j++)
			{
				int c = shGameBoard->gameBoard[i][j];
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
	UnmapViewOfFile(shGameBoard);
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
	
	if (shGameBoard->gameBoard[m->posY][m->posX] > 1000 && !hasItemIn(shGameBoard->gameBoard[m->posY][m->posX]) )
	{
		//_tprintf(TEXT("threat at Y:%d X: %d\nvalor no mapa:%d\nResto = %d\n"), m->posY, m->posX, shGameBoard->gameBoard[m->posY][m->posX], shGameBoard->gameBoard[m->posY][m->posX] % 100);
		//system("pause");
		m->hp++;
	}
}

void clrscr() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	const COORD startCoords = { 0, 0 };
	DWORD dummy;
	HANDLE hconsola = GetStdHandle(STD_OUTPUT_HANDLE);;

	GetConsoleScreenBufferInfo(hconsola, &csbi);
	FillConsoleOutputCharacter(hconsola,
		' ',
		csbi.dwSize.X * csbi.dwSize.Y,
		startCoords,
		&dummy);
	FillConsoleOutputAttribute(hconsola,
		csbi.wAttributes,
		csbi.dwSize.X * csbi.dwSize.Y,
		startCoords,
		&dummy);
	gotoxy(0, 0);
}

BOOL hasWallIn(int x, int y) {
	return shGameBoard->gameBoard[y][x] >= WALL_START_INDEX
		&& shGameBoard->gameBoard[y][x] <= WALL_END_INDEX;
}

BOOL hasMonsterIn(int x, int y) {
	return shGameBoard->gameBoard[y][x] >= MONSTER_START_INDEX
		&& shGameBoard->gameBoard[y][x] <= MONSTER_END_INDEX;
}

BOOL hasItemIn(int d) {
	
	int mp = (d - 501) / 100;
	
	
	if ((mp >= MONSTER_START_INDEX && mp <= MONSTER_END_INDEX))
	{
		
		return TRUE;
	}
	else if ((d % 100 >= ITEM_START_INDEX && d % 100 <= ITEM_END_INDEX))
	{
		
		return TRUE;
	}
	else
		return FALSE;
}

BOOL hasPlayerIn(int x, int y) {
	return shGameBoard->gameBoard[y][x] >= PLAYER_START_INDEX
		&& shGameBoard->gameBoard[y][x] <= PLAYER_END_INDEX;
}


//procura player
void CheckForPlayers(Monstro *m) {
	

	WaitForSingleObject(gMutexGameBoard, INFINITE);
	for (int i = 0; i < GAMEBOARDSIZE; i++)
	{
		for (int j = 0; j < GAMEBOARDSIZE; j++)
		{
			
			int c = shGameBoard->gameBoard[i][j];
			if (!hasItemIn(c) && hasPlayerIn(i, j))
			{
				if (i > m->posY)//se estiver abaixo
				{
					MoveMonstro(shGameBoard,DOWN,m);
					break;
				}
				else if (i < m->posY) //se estiver acima
				{
					MoveMonstro(shGameBoard, UP, m);
					break;
				}
				else
				{
					if (j > m->posX)// se estiver à direita
					{
						MoveMonstro(shGameBoard, LEFT, m);
						break;
					}
					else if (j < m->posX)//se estiver à esquerda
					{
						MoveMonstro(shGameBoard, RIGHT, m);
						break;
					}
				}
			}
			
		}
	}
	ReleaseMutex(gMutexGameBoard);
}

	