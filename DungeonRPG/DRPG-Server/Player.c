#include "Player.h"
#include "GameBoard.h"
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

	//Define x e y do jogador (pos vazia) e regista-o no gameBoard
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
*	Move-o para a posição desejada caso possível.
*/
void MovePlayer(int playerId, int keystroke) {
	Player* j = &gClients[playerId];

	if (!hasStamina(*j)) return;

	//GameBoard Ocupado - Bloqueia o acesso ao gameBoard por outras threads
	WaitForSingleObject(gMutexGameBoard, INFINITE);

	switch (keystroke) {
		case KEY_UP:
		{
			if (j->y > 1)
			{
				shGameBoard->gameBoard[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (shGameBoard->gameBoard[j->y][j->x] / 100) : EMPTY);
				if (!hasWallIn(j->x, j->y - 1) && !hasPlayerIn(j->x, j->y - 1)) j->y--;
				_tprintf(TEXT("Moving up...\n"));
			}
			break;
		}
		case KEY_DOWN:
		{
			if (j->y < GAMEBOARDSIZE-2)
			{
				shGameBoard->gameBoard[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (shGameBoard->gameBoard[j->y][j->x] / 100) : EMPTY);
				if (!hasWallIn(j->x, j->y + 1) && !hasPlayerIn(j->x, j->y + 1))j->y++;
				_tprintf(TEXT("Moving down...\n"));
			}
			break;
		}
		case KEY_LEFT:
		{
			if (j->x > 1)
			{
				shGameBoard->gameBoard[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (shGameBoard->gameBoard[j->y][j->x] / 100) : EMPTY);
				if (!hasWallIn(j->x - 1, j->y) && !hasPlayerIn(j->x - 1, j->y)) j->x--;
				_tprintf(TEXT("Moving left...\n"));
			}
			break;
		}
		case KEY_RIGHT:
		{
			if (j->x < GAMEBOARDSIZE - 2)
			{
				shGameBoard->gameBoard[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (shGameBoard->gameBoard[j->y][j->x] / 100) : EMPTY);
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
	shGameBoard->gameBoard[j->y][j->x] = playerId;
	
	//Liberta GameBoard - Desbloqueia o acesso ao gameBoard a outras threads
	ReleaseMutex(gMutexGameBoard);

	// Player is now "tired"
	j->speedCounter = j->movSpeed; //player is able to move on 0
	
	//Para efeitos de debug
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
	if (x + 5 > GAMEBOARDSIZE) maxX = GAMEBOARDSIZE;
	else maxX = x + 5;

	if (y - 5 < 0) iniY = 0;
	else iniY = y - 5;
	if (y + 5 > GAMEBOARDSIZE) maxY = GAMEBOARDSIZE;
	else maxY = y + 5;

	int m=0, n= 0;

	//GameBoard Ocupado - Bloqueia o acesso ao gameBoard por outras threads
	WaitForSingleObject(gMutexGameBoard, INFINITE);

	for (int i = iniY; i < maxY; i++, n++, m=0)
	{
		for (int j = iniX; j < maxX; j++, m++)
		{
			matriz[n][m] = shGameBoard->gameBoard[i][j];
		}
	}

	//Liberta GameBoard - Desbloqueia o acesso ao gameBoard a outras threads
	ReleaseMutex(gMutexGameBoard);
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
*	Coloca o jogador numa posição aleatória no gameBoard que esteja vazia.
*/
void SetPlayerInRandomPosition(Player* p) {
	int x, y;

	//GameBoard Ocupado - Bloqueia o acesso ao gameBoard por outras threads
	WaitForSingleObject(gMutexGameBoard, INFINITE);

	do {
		srand(time(NULL));
		x = (rand() % GAMEBOARDSIZE);
		y = (rand() % GAMEBOARDSIZE);
	} while (!(shGameBoard->gameBoard[y][x] == EMPTY));

	p->x = x;
	p->y = y;
	shGameBoard->gameBoard[y][x] = p->id;

	//Liberta GameBoard - Desbloqueia o acesso ao gameBoard a outras threads
	ReleaseMutex(gMutexGameBoard);
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
*	Reduz um "instante" na duração dos items do jogador.
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

	//GameBoard Ocupado - Bloqueia o acesso ao gameBoard por outras threads
	WaitForSingleObject(gMutexGameBoard, INFINITE);
	for (int i = (p->y - 1); i <= (p->y + 1); i++)
	{
		for (int j = (p->x - 1); j <= (p->x + 1); j++)
		{
			if (i == p->y && j == p->x) continue;
			if (hasPlayerIn(j, i) && gClients[shGameBoard->gameBoard[i][j]].hp > 0) {
				gClients[shGameBoard->gameBoard[i][j]].hp -= (UseStone(p) ? 2 : 1);
				p->atkCounter = (int)LENTIDAO_BASE;
				return;
			}
		}
	}
	//Liberta GameBoard - Desbloqueia o acesso ao gameBoard a outras threads
	ReleaseMutex(gMutexGameBoard);
}

/**
*	Verifica se existe algum monstro na mesma casa do jogador,
*	caso se verifique, o jogador é atacado.
*/
void CheckForThreats(Player* p) {
	//GameBoard Ocupado - Bloqueia o acesso ao gameBoard por outras threads
	WaitForSingleObject(gMutexGameBoard, INFINITE);

	if (shGameBoard->gameBoard[p->y][p->x] > 1000 && !hasItemIn(shGameBoard->gameBoard[p->y][p->x]))
		--p->hp;

	//Liberta GameBoard - Desbloqueia o acesso ao gameBoard a outras threads
	ReleaseMutex(gMutexGameBoard);
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
*	A posição do jogador no labririnto é preenchida com o número de pedras que este possui.
*/
void DropStones(Player* p) {
	int cellValue = (p->nStones > 0) ? (p->nStones * PEDRAS) : EMPTY;
	shGameBoard->gameBoard[p->y][p->x] = cellValue;
}

/**
* Item que está nas cordenadas actuais do jogador,
* mas que o movimento ainda não tenha sido registado na matriz global
* para ele poder obter o objecto.
* Depois de apanhado, a posição do objecto na matriz,
* deve de ser sobreposta com o id do utilizador.
*/
//Protegida pela função que a invoca. (Mutex)
void AskPlayerToCollectItems(Player* p) {
	int nPedras = 0;
	switch (shGameBoard->gameBoard[p->y][p->x])
	{
	case VITAMINA:
		if ((p->hp + 1) <= (int)HP_BASE * 2) p->hp++;
		break;
	case ORANGE_BULL:
		if ((p->hp + 1) <= (int)HP_BASE * 2) p->hp += 3;
		break;
	case REB_CAFEINA:
		p->movSpeed = (int)LENTIDAO_BASE - 2; //Efeito só deve de durar por 1 min (does not stack)
		p->itemDurationCounter = 15 * 60; // 15 instantes por segundo * 60 segundos = 900 instantes;
		break;
	default:
		if ((nPedras = shGameBoard->gameBoard[p->y][p->x]) > PEDRAS){
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
