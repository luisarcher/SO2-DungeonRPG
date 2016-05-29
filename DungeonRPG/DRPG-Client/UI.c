#include "UI.h"

BOOL iniciado = FALSE;

void enviaTecla(int k, HANDLE pipe) {
	ClientRequest req;
	int kp, seta = 0;
	switch (k)
	{
	case KEY_UP:
	{
		//_fgetts(req.msg, 256, stdin);
		//memset(req.msg, '\0', sizeof(TCHAR) * BUFFERSIZE);
		_tcscpy(req.msg, TEXT("UP"));
		req.command = (int)MOVEUP;
		EscreveMensagem(pipe, req);
		//LerMensagem(pipe);
		break;
	}
	case KEY_DOWN:
	{
		//_fgetts(req.msg, 256, stdin);
		//memset(req.msg, '\0', sizeof(TCHAR));
		_tcscpy(req.msg, TEXT("Down"));
		req.command = (int)MOVEDOWN;
		EscreveMensagem(pipe, req);
		//LerMensagem(pipe);
		break;
	}
	case KEY_LEFT:
	{
		//_fgetts(req.msg, 256, stdin);
		memset(req.msg, '\0', sizeof(TCHAR));
		_tcscpy(req.msg, TEXT("Left"));
		req.command = (int)MOVELEFT;
		EscreveMensagem(pipe, req);
		//LerMensagem(pipe);
		break;
	}
	case KEY_RIGHT:
	{
		//_fgetts(req.msg, 256, stdin);
		memset(req.msg, '\0', sizeof(TCHAR));
		_tcscpy(req.msg, TEXT("Right"));
		req.command = (int)MOVERIGHT;
		EscreveMensagem(pipe, req);
		//LerMensagem(pipe);
		break;
	}
	case KEY_ESCAPE:
	{
		iniciado = FALSE;

		clrscr();
		seta = 0;

		gotoxy(0, 0);
		MenuEscape(seta);

		while (iniciado == FALSE)
		{
			kp = Getch();
			switch (kp)
			{
			case KEY_UP:
				if (seta == 0)
				{
					seta = 1;
					MenuEscape(seta);
				}
				else
				{
					seta--;
					MenuEscape(seta);
				}
				break;
			case KEY_DOWN:
				if (seta == 1)
				{
					seta = 0;
					MenuEscape(seta);
				}
				else
				{
					seta++;
					MenuEscape(seta);
				}
				break;
			case KEY_ENTER:
				if (seta == 0)
				{
					iniciado = TRUE;
				}
				else if (seta == 1)
				{
					MenuInicial(0);
				}
				//break;
			default:
				//nope
				break;
			}
		}
		break;
	}
	case KEY_SPACE:
	{
		memset(req.msg, '\0', sizeof(TCHAR));
		_tcscpy(req.msg, TEXT("Activa Pedras\n"));
		req.command = (int)SWITCH_STONE_AUTOHIT;
		EscreveMensagem(pipe, req);

		break;
	}
	default:
		break;
	}
}

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

int Getch() {
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
			//return cChar;
			return -1;
		}

	return -1;
}

void MostraLOS(int(*los)[10]) {
	system("cls");

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			gotoxy(j, i);
			
			if (los[i][j] == STONEWALL)//paredes
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
				//_tprintf(TEXT("%c"), 'O');
				int item = los[i][j];
				if (item == VITAMINA) _tprintf(TEXT("%c"), '+');
				if (item == ORANGE_BULL) _tprintf(TEXT("%c"), '&');
				if (item == REB_CAFEINA) _tprintf(TEXT("%c"), '*');
			}
			else if (los[i][j] > PEDRAS)//objectos
			{
				_tprintf(TEXT("%c"), 'P');
			}
		}
		_tprintf(TEXT("\n"));
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