#pragma once
#include "./lib/ClientDLL.h"

//Tecla premida pelo utilizador
#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_LEFT 3
#define KEY_RIGHT 4
#define KEY_SPACE 5
#define KEY_ENTER 6
#define KEY_ESCAPE 7

#define EMPTY -1
//Paredes 20~29
#define WALL_START_INDEX 20
#define STONEWALL 20
#define WALL_END_INDEX 29

//Items 30~49 | 500+
#define ITEM_START_INDEX 30
#define VITAMINA	30	//+1 HP (Up to 200% base hp (200))
#define ORANGE_BULL 31	//+3 HP (Up to 200% base hp (200))
#define REB_CAFEINA 32	//-2 Lentidão (1min, não stackam)
#define PEDRAS 500		//+1 ataque quando usada
//pedras 50(quant), 500+ , ex 503 -> 3 pedras numa posição
#define ITEM_END_INDEX 49

extern iniciado;

void enviaTecla(int k, HANDLE pipe);
void MenuEscape(int op);
void MostraLOS(int(*los)[10]);
int Getch();
void MenuInicial(int op);
void clrscr();
void gotoxy(int x, int y);