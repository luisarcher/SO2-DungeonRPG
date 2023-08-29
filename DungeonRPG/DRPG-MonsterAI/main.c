#include "Monster.h"
#include "Common.h"

GameBoard* shGameBoard;
BOOL fim = FALSE;
HANDLE gMutexGameBoard;
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

	//debug
	//_tprintf(TEXT("Argc %d\nTipo %d\npassos %d\nHP %d \nx %d\ny %d\n"), argc, tipoMonstro, nPassos,hp,x, y);
	//system("pause");

	Monstro m;
	
#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	if ((gMutexGameBoard = OpenMutex(MUTEX_ALL_ACCESS, FALSE, TEXT("GameBoardOcupado"))) == NULL) {
		_tprintf(TEXT("[ERRO] Falha ao abir mutex de gameBoard. (%d)\n"), GetLastError());
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
	
	_tprintf(TEXT("Vou ler o estado do gameBoard...\n"));
	//system("pause");
	escondeCursor();
	//valida aqui o tipo de monstro
	if(tipoMonstro == 51){
		m = NovoMonstroBully(nPassos,hp,x,y);
		
	}
	else if (tipoMonstro == 52) {
		m = NovoMonstroDistraido(nPassos,hp,x,y);
	}
	
	/*m.posX = 15;
	m.posY = 1;*/
	
	
	shGameBoard->gameBoard[m.posY][m.posX] = m.tipo;
	
	//apenas move random-> implementar a perseguição

	while (!fim) { 
		//Espera pelo evento ser sinalizado do lado do servidor
		WaitForSingleObject(hGameInstanceEvent, INFINITE);
		//... Passou 1 instante... segue...

		//Mostra um raio de visão limitado
		DisplayMonsterSurroundings(m.posX, m.posY,m.hp); 
		
		if (m.stamina == 0)
		{
			if (m.passos == 0)
			{
				//ver percentagem 75%
				int r = rand() % 101;

				if (r <= 75)
				{
					m.direcao = MudaDirecao(shGameBoard, m.direcao, &m);
				}
				
				m.stamina = m.lentidao;
				m.passos = nPassos;
			}
			else {
				if (m.tipo = 51)
				{
					MoveMonstro(shGameBoard, m.direcao, &m);
					m.stamina = m.lentidao;
					m.passos--;
				}
				else {
					CheckForPlayers(&m);
					m.stamina = m.lentidao;
					m.passos--;
				}
				
			}
		}
		CheckForThreats(&m);
		
		
		
		if((m.hp*1.6) ==  (HP_BASE * 1.6))
		{
			//system("pause");
			TCHAR str[256];
			int hp = m.hp *0.80;
			
			
			if (!hasWallIn(m.posX + 1, m.posY) && !hasMonsterIn(m.posX+1,m.posY))
			{
				_stprintf_s(str, 256, TEXT("\DRPG-MonsterAI.exe %d %d %d %d %d"), m.tipo, m.passos, hp, m.posX + 1, m.posY);
			}
			else if(!hasWallIn(m.posX - 1, m.posY) && !hasMonsterIn(m.posX - 1, m.posY))
			{
				_stprintf_s(str, 256, TEXT("\DRPG-MonsterAI.exe %d %d %d %d %d"), m.tipo, m.passos, hp, m.posX - 1 , m.posY);
			}
			
			system("pause");
			ZeroMemory(&si, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			
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
			
			
			m.hp = hp;
		}
		
		
		m.stamina--;
		
	}

	CloseHandles(&hMappedObj);
}

