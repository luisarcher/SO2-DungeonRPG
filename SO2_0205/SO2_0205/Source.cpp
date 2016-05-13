#include <windows.h>
#include <tchar.h>
#include <fcntl.h>
#include <stdio.h>

float CalculaTempo(SYSTEMTIME inicio, SYSTEMTIME fim)
{
	LARGE_INTEGER nFim, nInicio;
	__int64 nFim_l, nInicio_l, nDiff;
	FILETIME fileTimeTemp;
	float fReturn = 0.0f;

	SystemTimeToFileTime(&fim, &fileTimeTemp);
	nFim.HighPart = fileTimeTemp.dwHighDateTime;
	nFim.LowPart = fileTimeTemp.dwLowDateTime;

	nFim_l = nFim.QuadPart; // fim as an int

	SystemTimeToFileTime(&inicio, &fileTimeTemp);
	nInicio.HighPart = fileTimeTemp.dwHighDateTime;
	nInicio.LowPart = fileTimeTemp.dwLowDateTime;
	nInicio_l = nInicio.QuadPart;

	nDiff = nFim_l - nInicio_l; // diferença deve ser positiva

	if (nDiff > 0)
	{
		_tprintf(TEXT("diferença > 0\n"));
		fReturn = nDiff / 10.2f; // microsegundos
	}
	else
	{
		_tprintf(TEXT("diferença <= 0\n"));
	}

	return fReturn;
}
int _tmain(int argc, TCHAR * argv[]) {
	HANDLE objMapeamento1, objMapeamento2, fich1, fich2;
	LARGE_INTEGER tamanho;
	SYSTEMTIME systime1, systime2;
	char * mapeamento1, *mapeamento2, *posActual1, *posActual2;
#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif
	if (argc != 3) {
		_tprintf(TEXT("[Erro]Sintaxe: %s ficheiro_origem ficheiro_destino\n"), argv[0]);
		return -1;
	}
	//Tentar abrir/criar os ficheiros passados pela linha de comandos
	fich1 = CreateFile(argv[1], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	fich2 = CreateFile(argv[2], GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fich1 == INVALID_HANDLE_VALUE || fich2 == INVALID_HANDLE_VALUE) {
		_tprintf(TEXT("[Erro] Abrir ficheiros (%d)\n"), GetLastError());
		return -1;
	}
	GetSystemTime(&systime1);
	_tprintf(TEXT("Início\t:%02d:%02d:%02d:%d\n"), systime1.wHour, systime1.wMinute,
		systime1.wSecond, systime1.wMilliseconds);
	//Saber o tamanho do ficheiro 1 em bytes
	GetFileSizeEx(fich1, &tamanho);

	//Cria um objecto para o mapeamento para cada ficheiro aberto 
	objMapeamento1 = CreateFileMapping(fich1, NULL, PAGE_READONLY, 0, 0, NULL);
	objMapeamento2 = CreateFileMapping(fich2, NULL, PAGE_READWRITE, tamanho.HighPart, tamanho.LowPart, NULL);
	if (objMapeamento1 == NULL || objMapeamento2 == NULL) {
		_tprintf(TEXT("[Erro]Criar objectos mapeamentos(%d)\n"), GetLastError());
		return -1;
	}

	//Mapear efectivamente o ficheiro em memória
	mapeamento1 = (char *)MapViewOfFile(objMapeamento1, FILE_MAP_READ, 0, 0, 0);
	mapeamento2 = (char *)MapViewOfFile(objMapeamento2, FILE_MAP_WRITE, 0, 0, tamanho.QuadPart);

	if (mapeamento1 == NULL || mapeamento2 == NULL) {
		_tprintf(TEXT("[Erro]Mapear para memória(%d)\n"), GetLastError());
		return -1;
	}
	posActual1 = mapeamento1;
	posActual2 = mapeamento2;

	//Percorrer o primeiro mapeamento e copiar para o segundo mapeamento
	while (posActual1 < mapeamento1 + tamanho.QuadPart) {
		*posActual2 = *posActual1;
		posActual1++;
		posActual2++;
	}
	UnmapViewOfFile(mapeamento1);
	UnmapViewOfFile(mapeamento2);
	CloseHandle(objMapeamento1);
	CloseHandle(objMapeamento2);
	GetSystemTime(&systime2);
	_tprintf(TEXT("Fim\t:%02d:%02d:%02d:%d\n"), systime2.wHour, systime2.wMinute,
		systime2.wSecond, systime2.wMilliseconds);
	_tprintf(TEXT("Tempo de execução em microsegundos: %10.2f\n"), CalculaTempo(systime1, systime2));
	CloseHandle(fich1);
	CloseHandle(fich2);
	return 0;
}
