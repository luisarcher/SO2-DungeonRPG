#include "Jogador.h"
#include "Labirinto.h"
#include "Common.h"
#include "Server.h"

/**
*	Regista um novo jogador ao ligar-se.
*/
void NovoJogador(int id) {
	Jogador * j = &gClients[id];
	_tcscpy(j->nome, TEXT("guest"));
	j->lentidao = (int)LENTIDAO_BASE;
	j->id = id;
	j->nStones = 0;
	j->stoneAutoHit = TRUE;

	j->lentidaoCounter = 0;
	j->atkCounter = 0;
	j->itemDurationCounter = 0;

	//Define x e y do jogador (pos vazia) e regista-o no labirinto
	SetPlayerInRandomPosition(j);

	//Define se o jogador é activo ou espectador, conforme o jogo estiver começado.
	j->hp = (!start) ? (int)HP_BASE : 0;
}

/**
*	Define o jogador como "inactivo".
*	Desliga e fecha os pipes do cliente.
*/
void DesligarJogador(Jogador * j) {
	j->hp = 0;
	DisconnectNamedPipe(j->hPipe);
	DisconnectNamedPipe(j->hPipeJogo);
	CloseHandle(j->hPipe);
	CloseHandle(j->hPipeJogo);
	_tprintf(TEXT("[SERVIDOR] Cliente [%d] desligou-se!\n"), j->id);
}

/**
*	Recebe a tecla premida pelo utilizador.
*	Move-o para a posição desejada caso possível.
*/
void MoverJogador(int playerId, int keystroke) {
	Jogador *j = &gClients[playerId];

	if (!hasStamina(*j)) return;

	WaitForSingleObject(gMutexLabirinto, INFINITE);
	switch (keystroke) {
		case KEY_UP:
		{
			if (j->y > 1)
			{
				shLabirinto->labirinto[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (shLabirinto->labirinto[j->y][j->x] / 100) : EMPTY);
				if (!hasWallIn(j->x, j->y - 1) && !hasPlayerIn(j->x, j->y - 1)) j->y--;
				_tprintf(TEXT("Moving up...\n"));
			}
			break;
		}
		case KEY_DOWN:
		{
			if (j->y < LABIRINTOSIZE-2)
			{
				shLabirinto->labirinto[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (shLabirinto->labirinto[j->y][j->x] / 100) : EMPTY);
				if (!hasWallIn(j->x, j->y + 1) && !hasPlayerIn(j->x, j->y + 1))j->y++;
				_tprintf(TEXT("Moving down...\n"));
			}
			break;
		}
		case KEY_LEFT:
		{
			if (j->x > 1)
			{
				shLabirinto->labirinto[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (shLabirinto->labirinto[j->y][j->x] / 100) : EMPTY);
				if (!hasWallIn(j->x - 1, j->y) && !hasPlayerIn(j->x - 1, j->y)) j->x--;
				_tprintf(TEXT("Moving left...\n"));
			}
			break;
		}
		case KEY_RIGHT:
		{
			if (j->x < LABIRINTOSIZE - 2)
			{
				shLabirinto->labirinto[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (shLabirinto->labirinto[j->y][j->x] / 100) : EMPTY);
				if (!hasWallIn(j->x + 1, j->y) && !hasPlayerIn(j->x + 1, j->y)) j->x++;
				_tprintf(TEXT("Moving right...\n"));
			}
			break;
		}
		default:
		{
			_tprintf(TEXT("[SERVER] Não foi possível movimentar o jogador!"));
			break;
		}
	}
	
	if (hasObjectIn(j->x, j->y)) AskPlayerToCollectItems(j);
	
	// Update matrix after collected items
	shLabirinto->labirinto[j->y][j->x] = playerId;
	
	ReleaseMutex(gMutexLabirinto);
	// Player is now "tired" and recovering stamina
	j->lentidaoCounter = j->lentidao; //player is able to move on 0
	
	_tprintf(TEXT("P[%d] -> POSX: %d POSY: %d\n\n"),playerId, j->x, j->y);
}

/**
*	Preenche a "parte visível" pelo utilizador e valida os limites do mapa.
*/
void UpdatePlayerLOS(int x, int y, int (*matriz)[PLAYER_LOS]) {
	SetEmptyMatrix(matriz);
	
	int iniX, iniY, maxX, maxY;

	if (x - 5 < 0) iniX = 0;
	else iniX = x - 5;
	if (x + 5 > LABIRINTOSIZE) maxX = LABIRINTOSIZE;
	else maxX = x + 5;

	if (y - 5 < 0) iniY = 0;
	else iniY = y - 5;
	if (y + 5 > LABIRINTOSIZE) maxY = LABIRINTOSIZE;
	else maxY = y + 5;

	int m=0, n= 0;
	for (int i = iniY; i < maxY; i++, n++, m=0)
	{
		for (int j = iniX; j < maxX; j++, m++)
		{
			matriz[n][m] = shLabirinto->labirinto[i][j];
		}
	}
}

/**
*	Preenche a matriz do jogador com "novoeiro".
*/
void SetEmptyMatrix(int (*matriz)[PLAYER_LOS]) {
	for (int i = 0; i < PLAYER_LOS; i++)
		for (int j = 0; j < PLAYER_LOS; j++)
			matriz[i][j] = FOG_OF_WAR;
}

/**
*	Coloca o jogador numa posição aleatória no labirinto que esteja vazia.
*/
void SetPlayerInRandomPosition(Jogador * p) {
	int x, y;
	do {
		srand(time(NULL));
		x = (rand() % LABIRINTOSIZE);
		y = (rand() % LABIRINTOSIZE);
	} while (!(shLabirinto->labirinto[y][x] == EMPTY));

	p->x = x;
	p->y = y;
	shLabirinto->labirinto[y][x] = p->id;
}

BOOL canAttack(Jogador p) {
	return (p.atkCounter == 0);
}

BOOL hasStamina(Jogador p) {
	return (p.lentidaoCounter == 0);
}

/**
*	O jogador recupera um ponto de stamina.
*/
void RecoverPlayerStamina(Jogador * p) {
	if (p->lentidaoCounter > 0)
		--p->lentidaoCounter;
	if (p->atkCounter > 0)
		--p->atkCounter;
}

/**
*	Reduz um "instante" na duração dos items do jogador.
*/
void CheckItemDurability(Jogador * p) {
	if (p->itemDurationCounter > 0)
		--p->itemDurationCounter;
	else {
		if (p->lentidao < LENTIDAO_BASE)
			p->lentidao = LENTIDAO_BASE;
	}
}

/**
*	Verifica se o jogador tem a possibilidade de atacar.
*	Verifica se existe algum jogador nas proximidades, se sim, ataca-o.
*/
void AttackClosePlayers(Jogador * p) {
	if (!canAttack(*p)) return;

	for (int i = (p->y - 1); i <= (p->y + 1); i++)
	{
		for (int j = (p->x - 1); j <= (p->x + 1); j++)
		{
			if (i == p->y && j == p->x) continue;
			if (hasPlayerIn(j, i) && gClients[shLabirinto->labirinto[i][j]].hp > 0) {
				gClients[shLabirinto->labirinto[i][j]].hp -= (UseStone(p) ? 2 : 1);
				p->atkCounter = (int)LENTIDAO_BASE;
				return;
			}
		}
	}
}

/**
*	Verifica se existe algum monstro na mesma casa do jogador,
*	caso se verifique, o jogador é atacado.
*/
void CheckForThreats(Jogador* p) {
	if (shLabirinto->labirinto[p->y][p->x] > 1000)
		--p->hp;
}

/**
*	O jogador usa uma das suas pedras para atacar.
*/
BOOL UseStone(Jogador * p) {
	if (p->stoneAutoHit == TRUE && p->nStones > 0) {
		--p->nStones;
		return TRUE;
	}
	return FALSE;
}

/**
*	A posição do jogador no labririnto é preenchida com o número de pedras que este possui.
*/
void DropStones(Jogador * p) {
	int cellValue = (p->nStones > 0) ? (p->nStones * PEDRAS) : EMPTY;
	shLabirinto->labirinto[p->y][p->x] = cellValue;
}

/**
* Item que está nas cordenadas actuais do jogador,
* mas que o movimento ainda não tenha sido registado na matriz global
* para ele poder obter o objecto.
* Depois de apanhado, a posição do objecto na matriz,
* deve de ser sobreposta com o id do utilizador.
*/
void AskPlayerToCollectItems(Jogador * p) {
	int nPedras = 0;
	switch (shLabirinto->labirinto[p->x][p->y])
	{
	case VITAMINA:
		if ((p->hp + 1) <= (int)HP_BASE * 2) p->hp++;
		break;
	case ORANGE_BULL:
		if ((p->hp + 1) <= (int)HP_BASE * 2) p->hp += 3;
		break;
	case REB_CAFEINA:
		p->lentidao = (int)LENTIDAO_BASE - 2; //Efeito só deve de durar por 1 min (does not stack)
		p->itemDurationCounter = 15 * 60; // 15 instantes por segundo * 60 segundos = 900 instantes;
		break;
	default:
		if ((nPedras = shLabirinto->labirinto[p->x][p->y]) > PEDRAS){
			nPedras -= (int)PEDRAS;
			if (p->nStones + nPedras <= (int)PLAYER_STONE_CAP) p->nStones += nPedras;
		}
		else {
			_tprintf(TEXT("[SERVER] Falha ao apanhar objecto!"));
		}
		break;
	}
}