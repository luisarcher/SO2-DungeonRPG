#include "GameData.h"

TCHAR * trataErrosPipe[3] = {
	TEXT("[0] Esperar pelo pipe - WaitNamedPipe"),
	TEXT("[1] Ligar ao pipe para pedidos - CreateFile"),
	TEXT("[2] Ligar ao pipe de broadcast - CreateFile")
};

TCHAR * _bitmaps[N_BITMAPS] = {
	MAKEINTRESOURCE(IDB_BITMAP6),	//0-Monstro distraido
	MAKEINTRESOURCE(IDB_BITMAP2),	//1-Monstro agressivo

	MAKEINTRESOURCE(IDB_BITMAP3),	//2-Relva
	MAKEINTRESOURCE(IDB_BITMAP4),	//3-Tijolo
	MAKEINTRESOURCE(IDB_BITMAP5),	//4-Granito

	MAKEINTRESOURCE(IDB_BITMAP7),	//5-Jogador

	MAKEINTRESOURCE(IDB_BITMAP8),	//6-Vitamina
	MAKEINTRESOURCE(IDB_BITMAP9),	//7-Orange Bull
	MAKEINTRESOURCE(IDB_BITMAP10),	//8-Reb Cafeina
	MAKEINTRESOURCE(IDB_BITMAP11)	//9-Pedras
};