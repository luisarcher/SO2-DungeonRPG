#pragma once
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>
#include <string>
using namespace std;

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

DWORD WINAPI EscrevePipe(LPVOID param);