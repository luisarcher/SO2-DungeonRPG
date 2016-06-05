#include "Monster.h"
#include "Common.h"

Labirinto * shLabirinto;
BOOL fim = FALSE;

int _tmain(int argc, LPTSTR argv[]) {
	HANDLE hMappedObj;
	HANDLE hGameInstanceEvent;
	int nPassos = 4; //recebe o argv adequado
	
#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	hGameInstanceEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, TEXT("gameInstanceEvent"));
	InitializeSharedMemory(&hMappedObj);
	
	_tprintf(TEXT("Vou ler o estado do labirinto...\n"));
	system("pause");
	escondeCursor();
	//valida aqui o tipo de monstro
	Monstro m = NovoMonstroBully(4);

	m.posX = 15;
	m.posY = 1;
	
	shLabirinto->labirinto[m.posY][m.posX] = m.tipo;
	
	while (!fim) { 
		//Espera pelo evento ser sinalizado do lado do servidor
		WaitForSingleObject(hGameInstanceEvent, INFINITE);
		//... Passou 1 instante... segue...

		ReadSharedMemory(); 
		
		if (m.stamina == 0)
		{
			if (m.passos == 0)
			{
				//ver percentagem 75%
				int r = rand() % 101;

				if (r <= 75)
				{
					m.direcao = MudaDirecao(shLabirinto, m.direcao, &m);
				}
				
				m.stamina = m.lentidao;
				m.passos = nPassos;
			}
			else {
				MoveMonstro(shLabirinto, m.direcao, &m);
				m.stamina = m.lentidao;
				m.passos--;
			}
		}
		CheckForThreats(&m);
		m.stamina--;
		//m.passos--;
	}

	CloseHandles(&hMappedObj);
}