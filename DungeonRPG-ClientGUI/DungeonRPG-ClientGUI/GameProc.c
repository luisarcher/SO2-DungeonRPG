#include "GameProc.h"
#include "MenuProc.h"
#include "SetupGame.h"
#include "GameData.h"
#include "Labirinto.h"
#include "UserInterface.h"

HDC hdcDoubleBuffer;

LRESULT CALLBACK WinProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	int result;

	HDC hdc;
	PAINTSTRUCT paintStruct;
	HBITMAP		hBitmap;

	RECT area, info;
	TCHAR buf[BUFFERSIZE];

	switch (messg)
	{
	case WM_CREATE:

		hdc = GetDC(hWnd);

		//Carregar bitmaps
		result = CarregarTodasAsImagens();
		if (result <= 0) {
			MessageBox(hWnd, TEXT("Erro ao carregar imagens"), TEXT("Carregar Imagens"), MB_OK | MB_ICONERROR);
		}

		ConfigurarDCs(hdc);

		ReleaseDC(hWnd, hdc);
		hdcDoubleBuffer = NULL;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtualizaJogo, (LPVOID)hWnd, 0, NULL);
		break;

	case WM_DESTROY:	

		PostQuitMessage(0);
		break;
	case WM_PAINT:
		//iniciar
		hdc = BeginPaint(hWnd, &paintStruct);

		//Depois define-se melhor a �rea de jogo
		//GetClientRect(hWnd, &area);

		if (hdcDoubleBuffer == NULL)
		{
			hdcDoubleBuffer = CreateCompatibleDC(hdc);
			hBitmap = CreateCompatibleBitmap(hdc, TILE_SZ*PLAYER_LOS, TILE_SZ*PLAYER_LOS);
			SelectObject(hdcDoubleBuffer, hBitmap);
		}
		
		area.left = 0;
		area.right = BOARD_LEFT_MARGIN + (PLAYER_LOS*TILE_SZ);
		area.top = 0;
		area.bottom = BOARD_TOP_MARGIN + (PLAYER_LOS*TILE_SZ);
		FillRect(hdcDoubleBuffer, &area, (HBRUSH)GetStockObject(WHITE_BRUSH));

		PrintBroadcastMessage(hdc);

		//Desenha interface do utilizador
		if (started) {
			DrawMap();					//Atrav�s da matriz recebida
			DrawInfo(hdc, area.right);	//Atrav�s da estrutura do jogador
		}
		
		//Copiar o que est� no buffer para o device context principal
		BitBlt(hdc,
			BOARD_LEFT_MARGIN,
			BOARD_TOP_MARGIN,
			TILE_SZ*PLAYER_LOS,
			TILE_SZ*PLAYER_LOS,
			hdcDoubleBuffer,
			0, 0,
			SRCCOPY);

		EndPaint(hWnd, &paintStruct);
		break;

	case WM_COMMAND:
		switch (wParam)
		{
		/* Menu Items*/
		case ID_FICHEIRO_LIGAR:
			DialogBox(ghInstance, MAKEINTRESOURCE(IDD_dlgMain), hWnd, DlgBoxConnectProc);
			break;
		case ID_FICHEIRO_COME40004: //Come�ar
			CallStartGame();
			break;
		/* Menu Items ends*/
		default:
			break;
		}

		break;
	
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_DOWN:
			EnviaTecla(MOVEDOWN);
			break;
		case VK_UP:
			EnviaTecla(MOVEUP);
			break;
		case VK_LEFT:
			EnviaTecla(MOVELEFT);
			break;
		case VK_RIGHT:
			EnviaTecla(MOVERIGHT);
			break;

		case WM_ERASEBKGND:		//N�o deixar que o fundo seja apagado (para t�cnica de BoubleBuffer)
			break;
		
		default:
			break;
		}
	default:
		// Neste exemplo, para qualquer outra mensagem (p.e. "minimizar", "maximizar",
		// "restaurar") n�o � efectuado nenhum processamento, apenas se segue 
		// o "default" do Windows DefWindowProc()
		return(DefWindowProc(hWnd, messg, wParam, lParam));
		break;
	}
	return(0);
}