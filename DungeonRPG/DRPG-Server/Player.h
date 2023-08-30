#ifndef DRPG_SERVER_PLAYER_H_
#define DRPG_SERVER_PLAYER_H_

#include "Common.h"

// Structure for internal use
typedef struct Player {
    TCHAR name[20];
    int x;
    int y;
    int id;
    int hp;
    int nStones;
    int movSpeed;

    int speedCounter;
    int atkCounter;
    int itemDurationCounter;

    BOOL stoneAutoHitToggle;
    HANDLE hPipe;
    HANDLE hPipeGame;
    HANDLE hThread;
} Player;

// Structure to inform the player
// Dedicated to avoid sending the entire structure with critical data to the server
typedef struct {
    int x;
    int y;
    int hp;
    int nStones;
    int speedCounter;
    int atkCounter;
    int itemDurationCounter;
    BOOL stoneAutoHitToggle;
} PlayerInformation;

// Registers a new player.
void NewPlayer(int id);

// Sets the player as "inactive" and closes pipes.
void DisconnectPlayer(Player* j);

// Moves the player based on the keystroke input.
void MovePlayer(int playerId, int keystroke);

// Updates the Line of Sight (LOS) matrix around the player's position.
void UpdatePlayerLOS(int x, int y, int(*matrix)[PLAYER_LOS]);

// Initializes the LOS matrix with empty values.
void SetEmptyMatrix(int(*matriz)[PLAYER_LOS]);

// Places the player in a random empty position on the gameBoard.
void SetPlayerInRandomPosition(Player* p);

// Asks the player to collect items on their current position.
void AskPlayerToCollectItems(Player* p);

// Recovers player stamina and attack counters.
void RecoverPlayerStamina(Player* p);

// Checks item durability and updates item effects.
void CheckItemDurability(Player* p);

// Attacks nearby players if conditions are met.
void AttackClosePlayers(Player* p);

// Checks for threats (monsters) and reduces player HP accordingly.
void CheckForThreats(Player* p);

// Uses a stone for attacking.
BOOL UseStone(Player* p);

// Checks if the player has stamina left.
BOOL hasStamina(Player p);

// Copies player data to a simplified structure.
PlayerInformation CopyPlayerData(Player p);

extern Player gClients[MAX_CLIENTS];

#endif // DRPG_SERVER_PLAYER_H_
