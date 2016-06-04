#pragma once
#include "Common.h"


#define MONSTRO_LENTIDAO_BASE 7
#define AGRESSIVO_DEFAULT 0

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
} Monstro;

//Monstro

Monstro NovoMonstroBully();

Monstro NovoMonstroDistraido();

void MoveMonstro(Labirinto *shLab, int d, Monstro *m);

int MudaDirecao(int anterior);

/*Memoria partilhada*/
void InitializeSharedMemory(HANDLE * hMappedObj);
void ReadSharedMemory();
void CloseHandles(HANDLE * hMappedObj);