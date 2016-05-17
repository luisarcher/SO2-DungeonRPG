#include "UI.h"

/*void MenuInicial()
{
	_tprintf(TEXT("teste"));
}*/
void gotoxy(int x, int y) {
	COORD coord;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(hStdout, coord);

}

void clrscr() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	const COORD startCoords = { 0, 0 };
	DWORD dummy;
	HANDLE hconsola = GetStdHandle(STD_OUTPUT_HANDLE);;

	GetConsoleScreenBufferInfo(hconsola, &csbi);
	FillConsoleOutputCharacter(hconsola,
		' ',
		csbi.dwSize.X * csbi.dwSize.Y,
		startCoords,
		&dummy);
	FillConsoleOutputAttribute(hconsola,
		csbi.wAttributes,
		csbi.dwSize.X * csbi.dwSize.Y,
		startCoords,
		&dummy);
	gotoxy(0, 0);


}

void MenuInicial(int op) {
	system("cls");
	//gotoxy(5, 5);

	switch (op) {
	case 0:
		_tprintf(TEXT(" >> Jogar\n\n"));
		_tprintf(TEXT("    Informacao\n\n"));
		_tprintf(TEXT("    Sair"));
		break;
	case 1:
		_tprintf(TEXT("    Jogar\n\n"));
		_tprintf(TEXT(" >> Informacao\n\n"));
		_tprintf(TEXT("    Sair"));
		break;
	case 2:
		_tprintf(TEXT("    Jogar\n\n"));
		_tprintf(TEXT("    Informacao\n\n"));
		_tprintf(TEXT(" >> Sair"));
	default:
		break;
	}
}

char Getch() {
	INPUT_RECORD irInputRecord;
	DWORD dwEventsRead;
	CHAR cChar;
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	while (ReadConsoleInputA(hStdin, &irInputRecord, 1, &dwEventsRead)) /* Read key press */
		if (irInputRecord.EventType == KEY_EVENT
			&& irInputRecord.Event.KeyEvent.wVirtualKeyCode != VK_SHIFT
			&& irInputRecord.Event.KeyEvent.wVirtualKeyCode != VK_MENU
			&& irInputRecord.Event.KeyEvent.wVirtualKeyCode != VK_CONTROL)
		{

			cChar = irInputRecord.Event.KeyEvent.uChar.AsciiChar;
			ReadConsoleInputA(hStdin, &irInputRecord, 1, &dwEventsRead); /* Read key release */

			if (irInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_LEFT) return KEY_LEFT;
			if (irInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT) return KEY_RIGHT;
			if (irInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_UP) return KEY_UP;
			if (irInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_DOWN) return KEY_DOWN;
			if (irInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_SPACE) return KEY_SPACE;
			if (irInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) return KEY_ENTER;
			if (irInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) return KEY_ESCAPE;
			return cChar;
		}

	return EOF;
}

void MostraLOS(int(*los)[10]) {
	//system("cls");

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			gotoxy(i, j);
			if (j == 9)
			{

				if (los[i][j] >= 20 && los[i][j] <= 29)//paredes
				{

					_tprintf(TEXT("%c\n"), 9619);
				}
				else if (los[i][j] == EMPTY)//chao
				{
					_tprintf(TEXT("%c\n"), 9617);
				}
				else if (los[i][j] >= 0 && los[i][j] <= 19)//players
				{
					_tprintf(TEXT("%c\n"), 64);
				}
				else if (los[i][j] >= 51 && los[i][j] <= 99)//monstros
				{
					_tprintf(TEXT("%c\n"), 167);
				}
				else if (los[i][j] >= 30 && los[i][j] <= 49)//objectos
				{
					_tprintf(TEXT("%c\n"), 'O');
				}
				else if (los[i][j] == 50)//objectos
				{
					_tprintf(TEXT("%c\n"), 'P');
				}

			}
			else
				if (los[i][j] >= 20 && los[i][j] <= 29)//paredes
				{
					_tprintf(TEXT("%c"), 9619);
				}
				else if (los[i][j] == EMPTY)//chao
				{
					_tprintf(TEXT("%c"), 9617);
				}
				else if (los[i][j] >= 0 && los[i][j] <= 19)//players
				{
					_tprintf(TEXT("%c"), 64);
				}
				else if (los[i][j] >= 51 && los[i][j] <= 99)//monstros
				{
					_tprintf(TEXT("%c"), 167);
				}
				else if (los[i][j] >= 30 && los[i][j] <= 49)//objectos
				{
					_tprintf(TEXT("%c"), 'O');
				}
				else if (los[i][j] == 50)//objectos
				{
					_tprintf(TEXT("%c"), 'P');
				}

		}
	}
}

void MenuEscape(int op) {
	clrscr();

	switch (op) {
	case 0:

	{
		_tprintf(TEXT(" >> Resumir\n\n"));
		//_tprintf(TEXT("    Informacao\n\n"));
		_tprintf(TEXT("    Sair"));
		break;
	}
	case 1:
		_tprintf(TEXT("  Resumir\n\n"));
		//_tprintf(TEXT("    Informacao\n\n"));
		_tprintf(TEXT(" >>  Sair"));
		break;
	default:
		break;
	}
}