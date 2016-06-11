#include "Proc.h"
#include "MenuProc.h"
#include "SetupGame.h"
#include "GameData.h"

LRESULT CALLBACK WinProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	int result;

	HDC hdc;
	HDC hdcDoubleBuffer = NULL;

	PAINTSTRUCT paintStruct;
	HBITMAP		hBitmap;

	RECT area;
	static int x = 0, y = 0;

	/* """"""""""""""""" garbage */
	HDC auxdc;
	static UINT eventoImagem;
	int tamx, tamy;
	static int xi = 0, xf = 0, yi = 0, yf = 0;
	static TCHAR letra[200] = TEXT("texto");
	TCHAR pal[100];
	/* """""""""""""""""""""" garbage ends*/

	switch (messg)
	{
	case WM_CREATE:

		//Carregar bitmaps
		result = CarregarTodasAsImagens();
		if (result < 0)
			MessageBox(hWnd, TEXT("Erro ao carregar imagens"), TEXT("Carregar Imagens"), MB_OK | MB_ICONERROR);
		
		hdc = GetDC(hWnd);
		ConfigurarDCs(hdc);
		ReleaseDC(hWnd, hdc);
		break;

	case WM_DESTROY:	

		PostQuitMessage(0);
		break;
	case WM_PAINT:
		//iniciar
		hdc = BeginPaint(hWnd, &paintStruct);

		//Depois define-se melhor a área de jogo
		//GetClientRect(hWnd, &area);

		//Preparação de DoubleBuffer
		if (hdcDoubleBuffer == NULL)
		{
			hdcDoubleBuffer = CreateCompatibleDC(hdc);
			hBitmap = CreateCompatibleBitmap(hdc, area.right, area.right);
			SelectObject(hdcDoubleBuffer, hBitmap);
		}

		//Desenho da área de jogo
		Rectangle(hdc, BOARD_LEFT_MARGIN, BOARD_TOP_MARGIN, PLAYER_LOS*TILE_SZ, PLAYER_LOS*TILE_SZ);
		//SetBkMode(hdc, TRANSPARENT);

		//DEBUBG - inicializar a matriz resp que é a resposta do servidor
		for (int i = 0; i < PLAYER_LOS; i++)
		{
			for (int j = 0; j < PLAYER_LOS; j++)
			{
				if (i == 0 || j == 0 || i == PLAYER_LOS-1 || j == PLAYER_LOS-1)
				{
					resp.matriz[i][j] = 20;
				}
				else resp.matriz[i][j] = -1;
			}
		}

		//Desenhar área de jogo no segundo buffer
		for (int i = 0; i < PLAYER_LOS; i++)
		{
			for (int j = 0; j < PLAYER_LOS; j++)
			{
				switch (resp.matriz[i][j])
				{
				case WALL_START_INDEX:
					BitBlt(
						hdcDoubleBuffer,
						i*(TILE_SZ / 2) + BOARD_TOP_MARGIN,
						j*(TILE_SZ / 2) + BOARD_LEFT_MARGIN,
						TILE_SZ, TILE_SZ,
						bitmapElementsDC[GRANITE],
						0, 0,
						SRCAND);
					break;
				case EMPTY:
					BitBlt(
						hdcDoubleBuffer,
						i*(TILE_SZ / 2) + BOARD_TOP_MARGIN,
						j*(TILE_SZ / 2) + BOARD_LEFT_MARGIN,
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

		//Copiar o que está no buffer para o device context principal
		BitBlt(hdc, 0, 0, area.right, area.bottom, hdcDoubleBuffer, 0, 0, SRCCOPY);


		//TextOut(hdc, 100, 100, TEXT("Olá"), 3);

		//mostrar bitmap em x,y
		//auxdc = CreateCompatibleDC(hdc);
		//SelectObject(auxdc, figura1);
		//BitBlt(hdc, 60, 60, 59, 59, auxdc, 0, 0, SRCCOPY);
		//StretchBlt(hdc, 0, 0, 32, 32, auxdc, 60, 60, 59, 59, SRCPAINT);

		//TransparentBlt(hdc, x, y, 64, 64, auxdc, 0, 0, 64, 64, RGB(255, 255, 255));


		//FIM
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
			//y++;
			y += 5;
			InvalidateRect(hWnd, NULL, 1);
			break;
		case VK_UP:
			//y--;
			y -= 5;
			InvalidateRect(hWnd, NULL, 1);
			break;
		case VK_LEFT:
			//--x;
			x -= 5;
			InvalidateRect(hWnd, NULL, 1);
			break;
		case VK_RIGHT:
			//++x;
			x += 5;
			InvalidateRect(hWnd, NULL, 1);
			break;

			//traduzir para o case
			/* notas aula 0606*/
			//Evitar flicks
			/*if (eventoImagem == ID_REPAINTOTIMIZADO || eventoImagem == ID_TRANSPARENTE) {
			area.bottom = y + 143 + 6;	//(y+1) * altura;
			area.left = x - 124 - 6;	//(x-1) * largura;
			area.right = x + 124 + 6;	//(x+1)	* largura;
			area.top = y - 143 - 6;		//(x-1) * altura;
			InvalidateRect(hwnd, &area, 0);
			}
			else InvalidateRect(hWnd, NULL, 1);*/

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