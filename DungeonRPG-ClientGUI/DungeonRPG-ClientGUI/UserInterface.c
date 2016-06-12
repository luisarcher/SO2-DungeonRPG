#include "UserInterface.h"
#include "GameData.h"
#include "SetupGame.h"
#include "Controller.h"

void PrintBroadcastMessage(HDC hdc) {
	TextOut(hdc, 2, 2, receivedMSG, BUFFERSIZE);
}

void DrawInfo(HDC hdc, int xStart) {
	//Desenho dos dados
	int _infoLeft = xStart + BOARD_LEFT_MARGIN;
	int _infoTop = BOARD_TOP_MARGIN;
	int _row = 1;

	TCHAR buf[BUFFERSIZE];

	TextOut(hdc, _infoLeft, _infoTop*_row, TEXT("Coordenadas:"), 12);	++_row;
	_stprintf(buf, TEXT("%d %d"), resp.playerInfo.x, resp.playerInfo.y);
	TextOut(hdc, _infoLeft, _infoTop*_row, buf, 5);	++_row;

	++_row;
	TextOut(hdc, _infoLeft, _infoTop*_row, TEXT("HP:"), 3);	++_row;
	_stprintf(buf, TEXT("%d"), resp.playerInfo.hp);
	TextOut(hdc, _infoLeft, _infoTop*_row, buf, 2);	++_row;

	++_row;
	TextOut(hdc, _infoLeft, _infoTop*_row, TEXT("Pedras:"), 7);	++_row;
	_stprintf(buf, TEXT("%d"), resp.playerInfo.nStones);
	TextOut(hdc, _infoLeft, _infoTop*_row, buf, 2);	++_row;
	if (resp.playerInfo.stoneAutoHit)
		_tcscpy(buf, TEXT("AutoAtaque ON"));
	else _tcscpy(buf, TEXT("AutoAtaque OFF"));
	TextOut(hdc, _infoLeft, _infoTop*_row, buf, 14);	++_row;

	++_row;
	TextOut(hdc, _infoLeft, _infoTop*_row, TEXT("Acções Permitidas:"), 18);	++_row;
	if (!resp.playerInfo.lentidaoCounter)
		_tcscpy(buf, TEXT("Mover"));
	TextOut(hdc, _infoLeft, _infoTop*_row, buf, 5);	++_row;
	if (!resp.playerInfo.atkCounter)
		_tcscpy(buf, TEXT("Atacar"));
	TextOut(hdc, _infoLeft, _infoTop*_row, buf, 6);	++_row;

	++_row;
	TextOut(hdc, _infoLeft, _infoTop*_row, TEXT("Items temporários:"), 18);	++_row;
	_stprintf(buf, TEXT("%d"), resp.playerInfo.itemDurationCounter);
	TextOut(hdc, _infoLeft, _infoTop*_row, buf, 3);	++_row;
}

void DrawMap() {
	for (int i = 0; i < PLAYER_LOS; i++)
	{
		for (int j = 0; j < PLAYER_LOS; j++)
		{
			int pos = resp.matriz[i][j];
			//Items sem opacidade
			if (isWall(pos) || pos == EMPTY || pos == FOG_OF_WAR)
			{
				switch (pos)
				{
				case EMPTY:
					BitBlt(
						hdcDoubleBuffer,
						j*(TILE_SZ)+BOARD_LEFT_MARGIN,
						i*(TILE_SZ)+BOARD_TOP_MARGIN,
						TILE_SZ, TILE_SZ,
						bitmapElementsDC[GRASS_I],
						0, 0,
						SRCAND);
					break;

				case BRICK:
					BitBlt(
						hdcDoubleBuffer,
						j*(TILE_SZ)+BOARD_LEFT_MARGIN,
						i*(TILE_SZ)+BOARD_TOP_MARGIN,
						TILE_SZ, TILE_SZ,
						bitmapElementsDC[BRICK_I],
						0, 0,
						SRCAND);
					break;

				case FOG_OF_WAR:
					Rectangle(hdcDoubleBuffer,
						(j*TILE_SZ) + BOARD_LEFT_MARGIN,
						(i*TILE_SZ) + BOARD_TOP_MARGIN,
						((j + 1)*TILE_SZ) + BOARD_LEFT_MARGIN,
						((i + 1)*TILE_SZ) + BOARD_TOP_MARGIN);
					break;
				default:
					break;
				}
			}
			else if (isObject(pos) || isPlayer(pos) || isMonsterAndPlayer(pos)
				|| pos == 51 || pos == 52) { //Items com opacidade
											 //Imprimir o chão que está por baixo do item
				BitBlt(
					hdcDoubleBuffer,
					j*(TILE_SZ)+BOARD_LEFT_MARGIN,
					i*(TILE_SZ)+BOARD_TOP_MARGIN,
					TILE_SZ, TILE_SZ,
					bitmapElementsDC[GRASS_I],
					0, 0,
					SRCAND);
				if (pos == MONSTER_A)
				{
					TransparentBlt(hdcDoubleBuffer,
						j*(TILE_SZ)+BOARD_LEFT_MARGIN,
						i*(TILE_SZ)+BOARD_TOP_MARGIN,
						TILE_SZ, TILE_SZ,
						bitmapElementsDC[MONSTER_A_I], 0, 0,
						TILE_SZ, TILE_SZ, RGB(255, 255, 255));
				}
				else if (pos == MONSTER_B) {
					TransparentBlt(hdcDoubleBuffer,
						j*(TILE_SZ)+BOARD_LEFT_MARGIN,
						i*(TILE_SZ)+BOARD_TOP_MARGIN,
						TILE_SZ, TILE_SZ,
						bitmapElementsDC[MONSTER_B_I], 0, 0,
						TILE_SZ, TILE_SZ, RGB(255, 255, 255));
				} //fim tipo monstro
				else
					if (isObject(pos)) //Qual objecto?
					{
						if (pos >= PEDRAS && pos< 600) { //É uma pedra?
							TransparentBlt(hdcDoubleBuffer,
								j*(TILE_SZ)+BOARD_LEFT_MARGIN,
								i*(TILE_SZ)+BOARD_TOP_MARGIN,
								TILE_SZ, TILE_SZ,
								bitmapElementsDC[STONES_I], 0, 0,
								//j*(TILE_SZ)+BOARD_LEFT_MARGIN, i*(TILE_SZ)+BOARD_TOP_MARGIN,
								TILE_SZ, TILE_SZ, RGB(255, 255, 255));
							continue; //continuar para o próximo for
						}

						switch (pos)
						{
						case VITAMINA:
							TransparentBlt(hdcDoubleBuffer,
								j*(TILE_SZ)+BOARD_LEFT_MARGIN,
								i*(TILE_SZ)+BOARD_TOP_MARGIN,
								TILE_SZ, TILE_SZ,
								bitmapElementsDC[VITAMINA_I], 0, 0,
								//j*(TILE_SZ)+BOARD_LEFT_MARGIN, i*(TILE_SZ)+BOARD_TOP_MARGIN,
								TILE_SZ, TILE_SZ, RGB(255, 255, 255));
							break;
						case ORANGE_BULL:
							TransparentBlt(hdcDoubleBuffer,
								j*(TILE_SZ)+BOARD_LEFT_MARGIN,
								i*(TILE_SZ)+BOARD_TOP_MARGIN,
								TILE_SZ, TILE_SZ,
								bitmapElementsDC[ORANGE_BULL_I], 0, 0,
								//j*(TILE_SZ)+BOARD_LEFT_MARGIN, i*(TILE_SZ)+BOARD_TOP_MARGIN,
								TILE_SZ, TILE_SZ, RGB(255, 255, 255));
							break;
						case REB_CAFEINA:
							TransparentBlt(hdcDoubleBuffer,
								j*(TILE_SZ)+BOARD_LEFT_MARGIN,
								i*(TILE_SZ)+BOARD_TOP_MARGIN,
								TILE_SZ, TILE_SZ,
								bitmapElementsDC[REB_CAFEINA_I], 0, 0,
								//j*(TILE_SZ)+BOARD_LEFT_MARGIN, i*(TILE_SZ)+BOARD_TOP_MARGIN,
								TILE_SZ, TILE_SZ, RGB(255, 255, 255));
							break;
						default:
							break;
						}
					} //fim objectos
					else if (isPlayer(pos)) {
						TransparentBlt(hdcDoubleBuffer,
							j*(TILE_SZ)+BOARD_LEFT_MARGIN - 1,
							i*(TILE_SZ)+BOARD_TOP_MARGIN - 1,
							TILE_SZ + 1, TILE_SZ + 1,
							bitmapElementsDC[PLAYER_I], 0, 0,
							//j*(TILE_SZ)+BOARD_LEFT_MARGIN, i*(TILE_SZ)+BOARD_TOP_MARGIN,
							TILE_SZ, TILE_SZ, RGB(56, 80, 80));
					} //fim player
			} //fim if objectos transparentes
		} //for j
	}// for i
}