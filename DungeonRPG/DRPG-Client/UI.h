#pragma once
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

void MenuInicial(int opt) {
	switch (opt) {
	case 0:
		_tprintf(TEXT(" >> Registar\n\n"));
		_tprintf(TEXT("    Login\n\n"));
		_tprintf(TEXT("    Sair"));
		break;
	case 1:
		_tprintf(TEXT("    Registar\n\n"));
		_tprintf(TEXT(" >> Login\n\n"));
		_tprintf(TEXT("    Sair"));
		break;
	case 2:
		_tprintf(TEXT("    Registar\n\n"));
		_tprintf(TEXT("    Login\n\n"));
		_tprintf(TEXT(" >> Sair"));
		break;
	}
}

/*char Getch() {
	INPUT_RECORD irInputRecord;
	DWORD dwEventsRead;
	CHAR cChar;

	while (ReadConsoleInputA(hStdin, &irInputRecord, 1, &dwEventsRead)) /* Read key press */
		/*if (irInputRecord.EventType == KEY_EVENT
			&& irInputRecord.Event.KeyEvent.wVirtualKeyCode != VK_SHIFT
			&& irInputRecord.Event.KeyEvent.wVirtualKeyCode != VK_MENU
			&& irInputRecord.Event.KeyEvent.wVirtualKeyCode != VK_CONTROL)
		{

			cChar = irInputRecord.Event.KeyEvent.uChar.AsciiChar;
			ReadConsoleInputA(hStdin, &irInputRecord, 1, &dwEventsRead); /* Read key release */

			/*if (irInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_LEFT) return LEFT;
			if (irInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT) return RIGHT;
			if (irInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_UP) return UP;
			if (irInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_DOWN) return DOWN;
			if (irInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_SPACE) return SPACE;
			if (irInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) return ENTER;
			if (irInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) return ESCAPE;
			return cChar;
		}

	return EOF;
}*/