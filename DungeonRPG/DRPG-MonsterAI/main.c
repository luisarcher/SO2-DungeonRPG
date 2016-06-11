#include "Monster.h"
#include "Common.h"

Labirinto * shLabirinto;
BOOL fim = FALSE;
HANDLE gMutexLabirinto;
HANDLE ghUpdateGameClientEvent;

//monster call MonsterType MonsterNStep MonsterHpSet
int _tmain(int argc, LPTSTR argv[]) {
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	HANDLE hMappedObj;
	HANDLE hGameInstanceEvent;
	int tipoMonstro = _ttoi(argv[1]);
	int nPassos = _ttoi(argv[2]); 
	int hp = _ttoi(argv[3]);
	int x = _ttoi(argv[4]);
	int y = _ttoi(argv[5]);
	/*argv[1] = 1;
	argv[2] = 2;*/
	_tprintf(TEXT("Argc %d\nTipo %d\npassos %d\nHP %d \nx %d\ny %d\n"), argc, tipoMonstro, nPassos,hp,x, y);
	Monstro m;
	
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
	//system("pause");
	escondeCursor();
	//valida aqui o tipo de monstro
	if(tipoMonstro == 1){
		m = NovoMonstroBully(nPassos);
		
	}
	else if (tipoMonstro == 2) {
		m = NovoMonstroDistraido(nPassos);
	}
	
	m.posX = 15;
	m.posY = 1;
	
	
	shLabirinto->labirinto[m.posY][m.posX] = m.tipo;
	
	//apenas move random-> implementar a perseguição
	while (!fim) { 
		//Espera pelo evento ser sinalizado do lado do servidor
		WaitForSingleObject(hGameInstanceEvent, INFINITE);
		//... Passou 1 instante... segue...

		//Mostra um raio de visão limitado
		DisplayMonsterSurroundings(m.posX, m.posY); 
		
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
		
		
		/*if((m.hp*1.6) ==  (HP_BASE * 1.6))
		{
			TCHAR str[256];
			//monster call MonsterType MonsterNStep MonsterHpSet
			_stprintf_s(str, 256, TEXT("\DRPG-MonsterAI.exe %d %d %d"), m.tipo, m.passos,m.hp * 0.8);

			m.hp = m.hp * 0.8;
			ZeroMemory(&si, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			//GetModuleFileName(NULL, "./DRPG-MonsterAI",256);
			CreateProcess(
				NULL,
				str,
				NULL,
				NULL,
				0,
				CREATE_NEW_CONSOLE,
				NULL,
				NULL,
				&si,
				&pi);
			
			
		}*/
		
		
		m.stamina--;
		
	}

	CloseHandles(&hMappedObj);
}