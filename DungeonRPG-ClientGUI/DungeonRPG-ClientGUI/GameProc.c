#include "GameProc.h"
#include "MenuProc.h"
#include "SetupGame.h"
#include "GameData.h"

HDC hdcDoubleBuffer;

LRESULT CALLBACK WinProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	int result;

	HDC hdc;
	PAINTSTRUCT paintStruct;
	HBITMAP		hBitmap;

	RECT area;
	static int x = 0, y = 0;

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

		//Depois define-se melhor a área de jogo
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

		//Desenho da área de jogo
		//Rectangle(hdc, BOARD_LEFT_MARGIN, BOARD_TOP_MARGIN, PLAYER_LOS*TILE_SZ, PLAYER_LOS*TILE_SZ);
		//SetBkMode(hdc, TRANSPARENT);

		/*for (int i = 0; i < PLAYER_LOS; i++)
		{
			for (int j = 0; j < PLAYER_LOS; j++)
			{
				resp.matriz[i][j] = -1;
			}
		}*/

		//Copiar o que está no buffer para o device context principal

		//Desenhar área de jogo no segundo buffer
		//FAZER CORRESPONDER A MATRIZ RECEBIDA, AOS BITMAPS
		for (int i = 0; i < PLAYER_LOS; i++)
		{
			for (int j = 0; j < PLAYER_LOS; j++)
			{
				switch (resp.matriz[i][j])
				{
				case WALL_START_INDEX:
					BitBlt(
						hdcDoubleBuffer,
						i*(TILE_SZ) + BOARD_TOP_MARGIN,
						j*(TILE_SZ) + BOARD_LEFT_MARGIN,
						TILE_SZ, TILE_SZ,
						bitmapElementsDC[BRICK],
						0, 0,
						SRCAND);
					break;

				case EMPTY:
					BitBlt(
						hdcDoubleBuffer,
						i*(TILE_SZ) + BOARD_TOP_MARGIN,
						j*(TILE_SZ) + BOARD_LEFT_MARGIN,
						TILE_SZ, TILE_SZ,
						bitmapElementsDC[GRASS],
						0, 0,
						SRCAND);
					break;
				default:
					break;
				}
			}
		}// FIM DB*/
		
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
			InvalidateRect(hWnd, NULL, 0);
			break;
		case VK_UP:
			EnviaTecla(MOVEUP);
			InvalidateRect(hWnd, NULL, 0);
			break;
		case VK_LEFT:
			EnviaTecla(MOVELEFT);
			InvalidateRect(hWnd, NULL, 0);
			break;
		case VK_RIGHT:
			EnviaTecla(MOVERIGHT);
			InvalidateRect(hWnd, NULL, 0);
			break;

		case WM_ERASEBKGND:		//Não deixar que o fundo seja apagado (para técnica de BoubleBuffer)
			break;
		
		default:
			break;
		}
	default:
		// Neste exemplo, para qualquer outra mensagem (p.e. "minimizar", "maximizar",
		// "restaurar") não é efectuado nenhum processamento, apenas se segue 
		// o "default" do Windows DefWindowProc()
		return(DefWindowProc(hWnd, messg, wParam, lParam));
		break;
	}
	return(0);
}