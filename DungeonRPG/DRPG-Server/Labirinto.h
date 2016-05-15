#ifndef LABIRINTO_H_INCLUDED
#define LABIRINTO_H_INCLUDED

#include "Jogador.h"

typedef struct LABIRINTO Labirinto;

struct LABIRINTO {
	int labirinto[70][70];
	boolean init;
};

Labirinto NovoLabirinto();

void posicionaJogador(Jogador *j, Labirinto *l);

extern Labirinto gLabirinto;
#endif