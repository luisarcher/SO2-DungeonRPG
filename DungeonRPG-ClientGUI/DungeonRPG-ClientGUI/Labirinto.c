#include "Labirinto.h"

BOOL isWall(int pos){
	return pos >= WALL_START_INDEX
		&& pos <= WALL_END_INDEX;
}

BOOL isObject(int pos) {
	return (pos >= ITEM_START_INDEX
		&& pos <= ITEM_END_INDEX)
		|| (pos > PEDRAS //Ver common.h
		&& pos < 600);
}

BOOL isMonster(int pos) {
	return pos >= MONSTER_START_INDEX
		&& pos <= MONSTER_START_INDEX + 50;
}

BOOL isPlayer(int pos) {
	return pos >= PLAYER_START_INDEX
		&& pos <= PLAYER_END_INDEX;
}

BOOL isMonsterAndPlayer(int pos) {
	return pos > 1000;
}