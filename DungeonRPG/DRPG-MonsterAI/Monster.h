#pragma once
#include "Common.h"

#define MONSTRO_LENTIDAO_BASE 7
#define AGRESSIVO_DEFAULT 0

typedef struct {
	int lentidao;
	int agressivo;
} Monstro;

void InitializeSharedMemory(HANDLE * hMappedObj);
void ReadSharedMemory();
void CloseHandles(HANDLE * hMappedObj);