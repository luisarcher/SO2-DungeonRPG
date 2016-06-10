#include "Proc.h"
#include "MenuProc.h"

LRESULT CALLBACK WinProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	//handle for device context
	HDC hdc;
	HDC auxdc;

	static UINT eventoImagem;
	static HBITMAP figura, figura1, fundo1, fundo2;
	static HBRUSH fundo3;
	PAINTSTRUCT pt;
	RECT area;
	int tamx, tamy;
	static HPEN linha;	//handle para uma caneta
	static HBRUSH fundo;

	static int x = 0, y = 0;
	static int xi = 0, xf = 0, yi = 0, yf = 0;
	static TCHAR letra[200] = TEXT("texto");
	TCHAR pal[100];
	PAINTSTRUCT p;
	switch (messg) {
	case WM_CREATE:
		figura = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP3));
		figura1 = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP2));
		linha = CreatePen(PS_DOT, 1, RGB(255, 0, 0));
		fundo3 = CreateSolidBrush(RGB(0, 255, 0));
		fundo = CreatePatternBrush(figura);
		break;
	case WM_PAINT:
		//iniciar
		hdc = BeginPaint(hWnd, &pt);

		/*aula anterior
		//TextOut(hdc, x, y, letra, _tcslen(letra));
		//hdc = GetDC(hWnd);
		//Ellipse(hdc, xi, yi, xf, yf);
		//ReleaseDC(hWnd,hdc);
		*/

		/*aula 0606*/
		SelectObject(hdc, fundo);
		SelectObject(hdc, linha);
		//SelectObject(hdc, fundo3);
		Rectangle(hdc, 25, 15, PLAYER_LOS*TILE_SZ, PLAYER_LOS*TILE_SZ);

		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 100, 100, TEXT("Olá"), 3);

		//mostrar bitmap em x,y
		auxdc = CreateCompatibleDC(hdc);
		SelectObject(auxdc, figura1);
		//BitBlt(hdc, 60, 60, 59, 59, auxdc, 0, 0, SRCCOPY);
		//StretchBlt(hdc, 0, 0, 32, 32, auxdc, 60, 60, 59, 59, SRCPAINT);

		TransparentBlt(hdc, x, y, 59, 59, auxdc, 0, 0, 59, 59, RGB(255, 255, 255));

		DeleteDC(auxdc);

		//FIM
		EndPaint(hWnd, &p);
		break;
	case WM_COMMAND:
		if (wParam == ID_FICHEIRO_REGISTAR) {
			//MessageBox(hWnd, TEXT("Frase"), TEXT("Title"), MB_OK);
			DialogBox(ghInstance, MAKEINTRESOURCE(IDD_dlgMain), hWnd, DlgBoxConnectProc);
		}
		break;
	case WM_DESTROY:	// Destruir a janela e terminar o programa
						// "PostQuitMessage(Exit Status)"	

		DeleteObject(figura);
		DeleteObject(figura1);
		DeleteObject(fundo);
		DeleteObject(fundo1);
		DeleteObject(fundo2);
		DeleteObject(fundo3);

		PostQuitMessage(0);
		break;
		//case WM_LBUTTONDOWN: //SINGLE CLICK
	case WM_LBUTTONDBLCLK:	//DOUBLECLICK
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		InvalidateRect(hWnd, NULL, 1);
		break;

	case WM_LBUTTONDOWN:
		xi = LOWORD(lParam);
		yi = HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
		xf = LOWORD(lParam);
		yf = HIWORD(lParam);
		InvalidateRect(hWnd, NULL, 1);
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