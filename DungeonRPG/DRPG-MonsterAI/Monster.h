#pragma once
#include "Common.h"


#define MONSTRO_LENTIDAO_BASE 7
#define AGRESSIVO_DEFAULT 0
#define HP_BASE 10

/*Direcções*/
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

typedef struct {
	int lentidao;
	int agressivo;
	int hp;
	int posX;
	int posY;
	int tipo;
	int direcao;
	int stamina;
	int passos;
} Monstro;

//Monstro

Monstro NovoMonstroBully();

Monstro NovoMonstroDistraido();

void MoveMonstro(Labirinto *shLab, int d, Monstro *m);

int MudaDirecao(int anterior);

void CheckForThreats(Monstro *m);

/*Memoria partilhada*/
void InitializeSharedMemory(HANDLE * hMappedObj);
void ReadSharedMemory();
void CloseHandles(HANDLE * hMappedObj);

void gotoxy(int x, int y);
void escondeCursor();

BOOL hasPlayerIn(int x, int y);