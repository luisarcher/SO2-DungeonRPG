#include "Monster.h"
#include "Common.h"

Labirinto * shLabirinto;
BOOL fim = FALSE;
HANDLE gMutexLabirinto;
HANDLE ghUpdateGameClientEvent;

int _tmain(int argc, LPTSTR argv[]) {
	HANDLE hMappedObj;
	HANDLE hGameInstanceEvent;
	int nPassos = 4; //recebe o argv adequado
	
#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	if ((gMutexLabirinto = OpenMutex(MUTEX_ALL_ACCESS, FALSE, TEXT("LabirintoOcupado"))) == NULL) {
		_tprintf(TEXT("[ERRO] Falha ao abir mutex de labirinto. (%d)\n"), GetLastError());
		exit(-1);
	}
	if ((hGameInstanceEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT("gameInstanceEvent"))) == NULL) {
		_tprintf(TEXT("[ERRO] Falha ao abir evento de instâncias. (%d)\n"), GetLastError());
		exit(-1);
	}
	if ((ghUpdateGameClientEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT("ActualizarClientes"))) == NULL) {
		_tprintf(TEXT("[ERRO] Falha ao abir evento de difusão. (%d)\n"), GetLastError());
		exit(-1);
	}
	InitializeSharedMemory(&hMappedObj);
	
	_tprintf(TEXT("Vou ler o estado do labirinto...\n"));
	system("pause");
	escondeCursor();
	//valida aqui o tipo de monstro
	Monstro m = NovoMonstroBully(4);

	m.posX = 15;
	m.posY = 1;
	
	shLabirinto->labirinto[m.posY][m.posX] = m.tipo;
	
	//apenas move random-> implementar a perseguição
	while (!fim) { 
		//Espera pelo evento ser sinalizado do lado do servidor
		WaitForSingleObject(hGameInstanceEvent, INFINITE);
		//... Passou 1 instante... segue...

		//ReadSharedMemory(); //Monstra labirinto todo
		DisplayMonsterSurroundings(m.posX, m.posY); //Monstra um raio de visão limitado
		
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
		//meter a separação aqui
		/*
		
		if((m.hp*1.6) ==  (HP_BASE * 1.6))
		{
			m.hp = m.hp * 0.8;
			iniciar novo monstro, passar HP por argumento
			
		}
		
		*/
		m.stamina--;
		//m.passos--;
	}

	CloseHandles(&hMappedObj);
}