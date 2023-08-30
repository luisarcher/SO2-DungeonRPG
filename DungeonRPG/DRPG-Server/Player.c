#include "Player.h"
#include "GameBoard.h"
#include "Common.h"
#include "Server.h"

void NewPlayer(int id) {
	Player* j = &gClients[id];
	_tcscpy(j->name, TEXT("guest"));
	j->movSpeed = (int)BASE_SLOWNESS;
	j->id = id;
	j->nStones = 0;
	j->stoneAutoHitToggle = TRUE;

	j->speedCounter = 0;
	j->atkCounter = 0;
	j->itemDurationCounter = 0;

	// Define player's x and y (empty position) and register in the gameBoard
	if (!start) SetPlayerInRandomPosition(j);
	j->hp = 0;
}

void DisconnectPlayer(Player* j) {
	j->hp = 0;
	DisconnectNamedPipe(j->hPipe);
	DisconnectNamedPipe(j->hPipeGame);
	CloseHandle(j->hPipe);
	CloseHandle(j->hPipeGame);
	_tprintf(TEXT("[SERVER] Client [%d] has disconnected!\n"), j->id);
}

/**
 * Receives the key pressed by the user.
 * Moves the player to the desired position if possible.
 */
void MovePlayer(int playerId, int keystroke) {
	Player* j = &gClients[playerId];

	if (!hasStamina(*j)) return;

	// Occupied gameBoard - Blocks gameBoard access for other threads
	WaitForSingleObject(gMutexGameBoard, INFINITE);

	switch (keystroke) {
	case KEY_UP:
	{
		if (j->y > 1) {
			shGameBoard->gameBoard[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (shGameBoard->gameBoard[j->y][j->x] / 100) : EMPTY);
			if (!hasWallIn(j->x, j->y - 1) && !hasPlayerIn(j->x, j->y - 1)) j->y--;
			_tprintf(TEXT("Moving up...\n"));
		}
		break;
	}
	case KEY_DOWN:
	{
		if (j->y < GAMEBOARDSIZE - 2) {
			shGameBoard->gameBoard[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (shGameBoard->gameBoard[j->y][j->x] / 100) : EMPTY);
			if (!hasWallIn(j->x, j->y + 1) && !hasPlayerIn(j->x, j->y + 1)) j->y++;
			_tprintf(TEXT("Moving down...\n"));
		}
		break;
	}
	case KEY_LEFT:
	{
		if (j->x > 1) {
			shGameBoard->gameBoard[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (shGameBoard->gameBoard[j->y][j->x] / 100) : EMPTY);
			if (!hasWallIn(j->x - 1, j->y) && !hasPlayerIn(j->x - 1, j->y)) j->x--;
			_tprintf(TEXT("Moving left...\n"));
		}
		break;
	}
	case KEY_RIGHT:
	{
		if (j->x < GAMEBOARDSIZE - 2) {
			shGameBoard->gameBoard[j->y][j->x] = (hasMonsterAndPlayerIn(j->x, j->y) ? (shGameBoard->gameBoard[j->y][j->x] / 100) : EMPTY);
			if (!hasWallIn(j->x + 1, j->y) && !hasPlayerIn(j->x + 1, j->y)) j->x++;
			_tprintf(TEXT("Moving right...\n"));
		}
		break;
	}
	default:
	{
		_tprintf(TEXT("[SERVER] Unable to move the player!"));
		break;
	}
	}

	if (hasObjectIn(j->x, j->y)) AskPlayerToCollectItems(j);

	// Update matrix after collecting items
	shGameBoard->gameBoard[j->y][j->x] = playerId;

	// Release gameBoard - Unlocks gameBoard access for other threads
	ReleaseMutex(gMutexGameBoard);

	// Player is now "tired"
	j->speedCounter = j->movSpeed; // player is able to move after 0

	// For debug purposes
	_tprintf(TEXT("P[%d] -> POSX: %d POSY: %d\n\n"), playerId, j->x, j->y);
}

/**
 * Fills the "visible part" for the player and validates map boundaries.
 */
void UpdatePlayerLOS(int x, int y, int(*matrix)[PLAYER_LOS]) {
	SetEmptyMatrix(matrix);

	int iniX, iniY, maxX, maxY;

	if (x - 5 < 0) iniX = 0;
	else iniX = x - 5;
	if (x + 5 > GAMEBOARDSIZE) maxX = GAMEBOARDSIZE;
	else maxX = x + 5;

	if (y - 5 < 0) iniY = 0;
	else iniY = y - 5;
	if (y + 5 > GAMEBOARDSIZE) maxY = GAMEBOARDSIZE;
	else maxY = y + 5;

	int m = 0, n = 0;

	// Locked gameBoard - Blocks gameBoard access for other threads
	WaitForSingleObject(gMutexGameBoard, INFINITE);

	for (int i = iniY; i < maxY; i++, n++, m = 0) {
		for (int j = iniX; j < maxX; j++, m++) {
			matrix[n][m] = shGameBoard->gameBoard[i][j];
		}
	}

	// Release gameBoard - Unlocks gameBoard access for other threads
	ReleaseMutex(gMutexGameBoard);
}

/**
 * Fills the player's matrix with "fog of war".
 */
void SetEmptyMatrix(int(*matrix)[PLAYER_LOS]) {
	for (int i = 0; i < PLAYER_LOS; i++)
		for (int j = 0; j < PLAYER_LOS; j++)
			matrix[i][j] = FOG_OF_WAR;
}

/**
 * Places the player in a random position on the gameBoard that is empty.
 */
void SetPlayerInRandomPosition(Player* p) {
	int x, y;

	// Locked gameBoard - Blocks gameBoard access for other threads
	WaitForSingleObject(gMutexGameBoard, INFINITE);

	do {
		srand(time(NULL));
		x = (rand() % GAMEBOARDSIZE);
		y = (rand() % GAMEBOARDSIZE);
	} while (!(shGameBoard->gameBoard[y][x] == EMPTY));

	p->x = x;
	p->y = y;
	shGameBoard->gameBoard[y][x] = p->id;

	// Release gameBoard - Unlocks gameBoard access for other threads
	ReleaseMutex(gMutexGameBoard);
}

BOOL canAttack(Player p) {
	return (p.atkCounter == 0);
}

BOOL hasStamina(Player p) {
	return (p.speedCounter == 0);
}

/**
 * The player recovers one point of stamina and attack.
 */
void RecoverPlayerStamina(Player* p) {
	if (p->speedCounter > 0)
		--p->speedCounter;
	if (p->atkCounter > 0)
		--p->atkCounter;
}

/**
 * Reduces a "tick" in the player's item duration.
 */
void CheckItemDurability(Player* p) {
	if (p->itemDurationCounter > 0)
		--p->itemDurationCounter;
	else {
		if (p->movSpeed < BASE_SLOWNESS)
			p->movSpeed = BASE_SLOWNESS;
	}
}

/**
 * Checks if the player is able to attack.
 * Checks if there is any player nearby, if yes, attacks it.
 */
void AttackClosePlayers(Player* p) {
	if (!canAttack(*p)) return;

	// Locked gameBoard - Blocks gameBoard access for other threads
	WaitForSingleObject(gMutexGameBoard, INFINITE);

	for (int i = (p->y - 1); i <= (p->y + 1); i++) {
		for (int j = (p->x - 1); j <= (p->x + 1); j++) {
			if (i == p->y && j == p->x) continue;
			if (hasPlayerIn(j, i) && gClients[shGameBoard->gameBoard[i][j]].hp > 0) {
				gClients[shGameBoard->gameBoard[i][j]].hp -= (UseStone(p) ? 2 : 1);
				p->atkCounter = (int)BASE_SLOWNESS;
				return;
			}
		}
	}

	// Release gameBoard - Unlocks gameBoard access for other threads
	ReleaseMutex(gMutexGameBoard);
}

/**
 * Checks if there is a monster in the same cell as the player,
 * if so, the player is attacked.
 */
void CheckForThreats(Player* p) {
	// Locked gameBoard - Blocks gameBoard access for other threads
	WaitForSingleObject(gMutexGameBoard, INFINITE);

	if (shGameBoard->gameBoard[p->y][p->x] > 1000 && !hasItemIn(shGameBoard->gameBoard[p->y][p->x]))
		--p->hp;

	// Release gameBoard - Unlocks gameBoard access for other threads
	ReleaseMutex(gMutexGameBoard);
}

/**
 * The player uses one of their stones to attack.
 * The used stone is discarded from player's stone count.
 */
BOOL UseStone(Player* p) {
	if (p->stoneAutoHitToggle == TRUE && p->nStones > 0) {
		--p->nStones;
		return TRUE;
	}
	return FALSE;
}

/**
 * The player's position in the maze is filled with the number of stones they have.
 */
void DropStones(Player* p) {
	int cellValue = (p->nStones > 0) ? (p->nStones * STONES) : EMPTY;
	shGameBoard->gameBoard[p->y][p->x] = cellValue;
}

/**
 * Item at the current player's coordinates,
 * but the movement hasn't been registered in the global matrix yet
 * so they can obtain the object.
 * After collecting, the object's position in the matrix
 * should be overwritten with the user's ID.
 */
 // Protected by the invoking function. (Mutex)
void AskPlayerToCollectItems(Player* p) {
	int nStones = 0;
	switch (shGameBoard->gameBoard[p->y][p->x]) {
	case VITAMIN:
		if ((p->hp + 1) <= (int)BASE_HP * 2) p->hp++;
		break;
	case ORANGE_BULL:
		if ((p->hp + 1) <= (int)BASE_HP * 2) p->hp += 3;
		break;
	case REB_CAFFEINE:
		p->movSpeed = (int)BASE_SLOWNESS - 2;	// Effect should only last for 1 minute (does not stack)
		p->itemDurationCounter = 15 * 60;		// 15 ticks per second * 60 seconds = 900 ticks;
		break;
	default:
		if ((nStones = shGameBoard->gameBoard[p->y][p->x]) > STONES) {
			nStones -= (int)STONES;
			if (p->nStones + nStones <= (int)PLAYER_STONE_CAP) p->nStones += nStones;
		}
		else {
			_tprintf(TEXT("[SERVER] Failed to collect object!"));
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
	int monsterType = (d - 501) / 100;

	if ((monsterType >= MONSTER_START_INDEX && monsterType <= MONSTER_END_INDEX)) {
		return TRUE;
	}
	else if ((d % 100 >= ITEM_START_INDEX && d % 100 <= ITEM_END_INDEX)) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}
