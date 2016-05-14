#ifndef JOGADOR_H_INCLUDED
#define JOGADOR_H_INCLUDED
#include "Common.h"

typedef struct JOGADOR Jogador;
struct JOGADOR {
	TCHAR nome[20];
	int x;
	int y;
	int id;
	int hp;
	HANDLE hPipe;
	HANDLE hThread;
};

void NovoJogador(Jogador * j);
int MoverJogador(int playerId, int keystroke);

extern Jogador gClients[MAX_CLIENTS];
extern Jogador j; //eliminar
#endif