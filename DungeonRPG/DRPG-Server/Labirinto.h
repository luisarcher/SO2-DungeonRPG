#ifndef LABIRINTO_H_INCLUDED
#define LABIRINTO_H_INCLUDED

#include "Jogador.h"

typedef struct LABIRINTO Labirinto;
struct LABIRINTO {
	int labirinto[LABIRINTOSIZE][LABIRINTOSIZE];
	boolean init;
};

Labirinto NovoLabirinto();
Labirinto LerLabirinto();

extern Labirinto gLabirinto;
#endif