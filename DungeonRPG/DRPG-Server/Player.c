#include "Player.h"
#include "Labirinto.h"
#include "Common.h"
#include "Server.h"

/**
*	Registers a new player.
*/
void NewPlayer(int id) {
	Player* j = &gClients[id];
	_tcscpy(j->name, TEXT("guest"));
	j->movSpeed = (int)LENTIDAO_BASE;
	j->id = id;
	j->nStones = 0;
	j->stoneAutoHitToggle = TRUE;

	j->speedCounter = 0;
	j->atkCounter = 0;
	j->itemDurationCounter = 0;

	//Define x e y do jogador (pos vazia) e regista-o no labirinto
	if (!start) SetPlayerInRandomPosition(j);
	j->hp =  0;
}

/**
*	Define o jogador como "inactivo".
*	Desliga e fecha os pipes do cliente.
*/
void DisconnectPlayer(Player* j) {
	j->hp = 0;
	DisconnectNamedPipe(j->hPipe);
	DisconnectNamedPipe(j->hPipeGame);
	CloseHandle(j->hPipe);
	CloseHandle(j->hPipeGame);
	_tprintf(TEXT("[SERVIDOR] Cliente [%d] desligou-se!\n"), j->id);
}

/**
*	Recebe a tecla premida pelo utilizador.
*	Move-o para a posi��o desejada caso poss�vel.
*/
void MoverJogador(int playerId, int keystroke) {
	Player* j = &gClients[playerId];

	if (!hasStamina(*j)) return;

	//Labirinto Ocupado - Bloqueia o acesso ao labirinto por outras threads
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
			_tprintf(TEXT("[SERVER] N�o foi poss�vel movimentar o jogador!"));
			break;
		}
	}
	
	if (hasObjectIn(j->x, j->y)) AskPlayerToCollectItems(j);
	
	// Update matrix after collected items
	shLabirinto->labirinto[j->y][j->x] = playerId;
	
	//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
	ReleaseMutex(gMutexLabirinto);

	// Player is now "tired"
	j->speedCounter = j->movSpeed; //player is able to move on 0
	
	//Para efeitos de debug
	_tprintf(TEXT("P[%d] -> POSX: %d POSY: %d\n\n"),playerId, j->x, j->y);
}

/**
*	Preenche a "parte vis�vel" pelo utilizador e valida os limites do mapa.
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

	//Labirinto Ocupado - Bloqueia o acesso ao labirinto por outras threads
	WaitForSingleObject(gMutexLabirinto, INFINITE);

	for (int i = iniY; i < maxY; i++, n++, m=0)
	{
		for (int j = iniX; j < maxX; j++, m++)
		{
			matriz[n][m] = shLabirinto->labirinto[i][j];
		}
	}

	//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
	ReleaseMutex(gMutexLabirinto);
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
*	Coloca o jogador numa posi��o aleat�ria no labirinto que esteja vazia.
*/
void SetPlayerInRandomPosition(Player* p) {
	int x, y;

	//Labirinto Ocupado - Bloqueia o acesso ao labirinto por outras threads
	WaitForSingleObject(gMutexLabirinto, INFINITE);

	do {
		srand(time(NULL));
		x = (rand() % LABIRINTOSIZE);
		y = (rand() % LABIRINTOSIZE);
	} while (!(shLabirinto->labirinto[y][x] == EMPTY));

	p->x = x;
	p->y = y;
	shLabirinto->labirinto[y][x] = p->id;

	//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
	ReleaseMutex(gMutexLabirinto);
}

BOOL canAttack(Player p) {
	return (p.atkCounter == 0);
}

BOOL hasStamina(Player p) {
	return (p.speedCounter == 0);
}

/**
*	O jogador recupera um ponto de stamina e de ataque.
*/
void RecoverPlayerStamina(Player* p) {
	if (p->speedCounter > 0)
		--p->speedCounter;
	if (p->atkCounter > 0)
		--p->atkCounter;
}

/**
*	Reduz um "instante" na dura��o dos items do jogador.
*/
void CheckItemDurability(Player* p) {
	if (p->itemDurationCounter > 0)
		--p->itemDurationCounter;
	else {
		if (p->movSpeed < LENTIDAO_BASE)
			p->movSpeed = LENTIDAO_BASE;
	}
}

/**
*	Verifica se o jogador tem a possibilidade de atacar.
*	Verifica se existe algum jogador nas proximidades, se sim, ataca-o.
*/
void AttackClosePlayers(Player* p) {
	if (!canAttack(*p)) return;

	//Labirinto Ocupado - Bloqueia o acesso ao labirinto por outras threads
	WaitForSingleObject(gMutexLabirinto, INFINITE);
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
	//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
	ReleaseMutex(gMutexLabirinto);
}

/**
*	Verifica se existe algum monstro na mesma casa do jogador,
*	caso se verifique, o jogador � atacado.
*/
void CheckForThreats(Player* p) {
	//Labirinto Ocupado - Bloqueia o acesso ao labirinto por outras threads
	WaitForSingleObject(gMutexLabirinto, INFINITE);

	if (shLabirinto->labirinto[p->y][p->x] > 1000 && !hasItemIn(shLabirinto->labirinto[p->y][p->x]))
		--p->hp;

	//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
	ReleaseMutex(gMutexLabirinto);
}

/**
*	O jogador usa uma das suas pedras para atacar.
*/
BOOL UseStone(Player* p) {
	if (p->stoneAutoHitToggle == TRUE && p->nStones > 0) {
		--p->nStones;
		return TRUE;
	}
	return FALSE;
}

/**
*	A posi��o do jogador no labririnto � preenchida com o n�mero de pedras que este possui.
*/
void DropStones(Player* p) {
	int cellValue = (p->nStones > 0) ? (p->nStones * PEDRAS) : EMPTY;
	shLabirinto->labirinto[p->y][p->x] = cellValue;
}

/**
* Item que est� nas cordenadas actuais do jogador,
* mas que o movimento ainda n�o tenha sido registado na matriz global
* para ele poder obter o objecto.
* Depois de apanhado, a posi��o do objecto na matriz,
* deve de ser sobreposta com o id do utilizador.
*/
//Protegida pela fun��o que a invoca. (Mutex)
void AskPlayerToCollectItems(Player* p) {
	int nPedras = 0;
	switch (shLabirinto->labirinto[p->y][p->x])
	{
	case VITAMINA:
		if ((p->hp + 1) <= (int)HP_BASE * 2) p->hp++;
		break;
	case ORANGE_BULL:
		if ((p->hp + 1) <= (int)HP_BASE * 2) p->hp += 3;
		break;
	case REB_CAFEINA:
		p->movSpeed = (int)LENTIDAO_BASE - 2; //Efeito s� deve de durar por 1 min (does not stack)
		p->itemDurationCounter = 15 * 60; // 15 instantes por segundo * 60 segundos = 900 instantes;
		break;
	default:
		if ((nPedras = shLabirinto->labirinto[p->y][p->x]) > PEDRAS){
			nPedras -= (int)PEDRAS;
			if (p->nStones + nPedras <= (int)PLAYER_STONE_CAP) p->nStones += nPedras;
		}
		else {
			_tprintf(TEXT("[SERVER] Falha ao apanhar objecto!"));
		}
		break;
	}
}

PlayerInformation CopyPlayerData(Player p) {
	PlayerInformation _info;
	_info.atkCounter = p.atkCounter;
	_info.hp = p.hp;
	_info.itemDurationCounter = p.itemDurationCounter;
	_info.speedCounter = p.speedCounter;
	_info.nStones = p.nStones;
	_info.stoneAutoHitToggle = p.stoneAutoHitToggle;
	_info.x = p.x;
	_info.y = p.y;
	return _info;
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
