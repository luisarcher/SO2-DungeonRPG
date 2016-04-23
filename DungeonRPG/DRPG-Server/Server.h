#pragma once
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

//Permitir que o mesmo código possa funcionar para ASCII ou UNICODE
#ifdef UNICODE 
#define tcout wcout
#define tcin wcin
#define tcerr wcerr
#define tstring wstring
#else
#define tcout cout
#define tcin cin
#define tcerr cerr
#define tstring string
#endif

#define PIPE_NAME TEXT("\\\\.\\pipe\\teste")
#define MAX_CLIENTS 10
#define BUFFERSIZE 256

DWORD WINAPI RecebeClientes(LPVOID param);