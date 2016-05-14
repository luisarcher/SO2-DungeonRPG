#include "Jogador.h"

Jogador NovoJogador(TCHAR *n) {
	Jogador j;
	_tcscpy(j.nome, n);
	j.x = 0;
	j.y = 0;
	j.hp = 100;
	j.id = 0;
	return  j;
}