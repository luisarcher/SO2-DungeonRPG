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