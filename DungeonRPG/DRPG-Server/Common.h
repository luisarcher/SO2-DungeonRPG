#pragma once
#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

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

//Game Constants
#define PIPE_NAME TEXT("\\\\.\\pipe\\DRPG")
#define MAX_CLIENTS 10
#define BUFFERSIZE 256

//Game Globals
extern HANDLE pipeClients[MAX_CLIENTS];
extern BOOL fim;
extern int totalConnections;
extern HANDLE hconsola;
#endif
