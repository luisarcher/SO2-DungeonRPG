#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include "lib\ClientDLL.h"

#define PLAYER_LOS			10
#define TILE_SZ				64
#define BOARD_TOP_MARGIN	25
#define BOARD_LEFT_MARGIN	25

LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DlgBox1Proc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);

TCHAR *szProgName = TEXT("DungeonRPG");

HINSTANCE ghInstance;

//Handlers para pipes
HANDLE hPipe;
HANDLE hPipeJogo;

// ============================================================================
// FUNÇÂO DE INÍCIO DO PROGRAMA: WinMain()
// ============================================================================
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {

	HWND hWnd;			// hWnd é o handler da janela, gerado mais abaixo por CreateWindow()
	MSG lpMsg;			// MSG é uma estrutura definida no Windows para as mensagens
	WNDCLASSEX wcApp;	// WNDCLASSEX é uma estrutura cujos membros servem para definir as características da classe da janela
	ghInstance = hInst;

	/* Definição das características da janela */
	wcApp.cbSize = sizeof(WNDCLASSEX);	// Tamanho da estrutura WNDCLASSEX
	wcApp.hInstance = hInst;			// Instância da janela actualmente exibida ("hInst" é parâmetro de WinMain e vem inicializada daí).
	wcApp.lpszClassName = szProgName;	// Nome da janela (neste caso = nome do programa)
	wcApp.lpfnWndProc = WinProc;		// Endereço da função de processamento da janela
	wcApp.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;			// Estilo da janela: Fazer o redraw vertical, horizontar, e poder capturar duplo Cliques.
	wcApp.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// "hIcon" = handler do ícon normal | "NULL" = Icon definido no Windows | "IDI_AP..." Ícone "aplicação"
	wcApp.hIconSm = LoadIcon(NULL, IDI_INFORMATION);			// "hIcon" = handler do ícon pequeno | Ver documentação prof.
	wcApp.hCursor = LoadCursor(NULL, IDC_ARROW);				// "hCursor" = handler do cursor (rato) | Ver documentação prof.
	wcApp.lpszMenuName = IDR_MENU1;							// Classe do menu que a janela pode ter | (NULL = não tem menu)
	wcApp.cbClsExtra = 0;							// Livre, para uso particular
	wcApp.cbWndExtra = 0;							// Livre, para uso particular
	wcApp.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	/*OUTRAS CORES DE BRUSH:	BLACK_BRUSH  DKGRAY_BRUSH GRAY_BRUSH LTGRAY_BRUSH  */

	// ============================================================================
	// 2. Registar a classe "wcApp" no Windows
	// ============================================================================
	if (!RegisterClassEx(&wcApp))
		return(0);

	// ============================================================================
	// 3. Criar a janela
	// ============================================================================
	hWnd = CreateWindow(
		szProgName,				// Nome da janela (programa) definido acima
		TEXT("Dungeon RPG - Sistemas Operativos 2"),	// Texto que figura na barra da janela
		WS_OVERLAPPEDWINDOW,	// Estilo da janela (WS_OVERLAPPED= normal)
								// Outros valores: WS_HSCROLL, WS_VSCROLL
								// (Fazer o OR "|" do que se pretender)
		CW_USEDEFAULT,			// Posição x pixels (default=à direita da última)
		CW_USEDEFAULT,			// Posição y pixels (default=abaixo da última)
		CW_USEDEFAULT,			// Largura da janela (em pixels)
		CW_USEDEFAULT,			// Altura da janela (em pixels)
		(HWND)HWND_DESKTOP,	// handle da janela pai (se se criar uma a partir 
							// de outra) ou HWND_DESKTOP se a janela for
							// a primeira, criada a partir do "desktop"
		(HMENU)NULL,			// handle do menu da janela (se tiver menu)
		(HINSTANCE)hInst,			// handle da instância do programa actual
									// ("hInst" é declarado num dos parâmetros
									// de WinMain(), valor atribuído pelo Windows)
		0);			// Não há parâmetros adicionais para a janela

	// ============================================================================
	// 4. Mostrar a janela
	// ============================================================================
	ShowWindow(hWnd, nCmdShow);	// "hWnd"= handler da janela, devolvido por "CreateWindow";
	UpdateWindow(hWnd);			// Refrescar a janela (Windows envia à janela uma mensagem para pintar, mostrar dados, (refrescar), etc)

	// ============================================================================
	// 5. Loop de Mensagens
	// ============================================================================
								// O Windows envia mensagens às janelas (programas). Ver restante documentação.| NOTA: GetMessage() devolve 0 quando for recebida a mensagem de fecho da janela.
	while (GetMessage(&lpMsg, NULL, 0, 0)) {
		TranslateMessage(&lpMsg);			// Pré-processamento da mensagem
											// p.e. obter código ASCII da tecla premida
		DispatchMessage(&lpMsg);			// Enviar a mensagem traduzida de volta ao Windows, que aguarda até que a 
											// possa reenviar à função de tratamento da janela, CALLBACK TrataEventos (mais abaixo).
	}
	return((int)lpMsg.wParam);		// Retorna-se sempre o parâmetro "wParam" da
									// estrutura "lpMsg"
}

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
		fundo3 = CreateSolidBrush(RGB(0,255,0));
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
		SelectObject(hdc,fundo);
		SelectObject(hdc,linha);
		//SelectObject(hdc, fundo3);
		Rectangle(hdc,25, 15, PLAYER_LOS*TILE_SZ, PLAYER_LOS*TILE_SZ);

		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 100, 100, TEXT("Olá"), 3);

		//mostrar bitmap em x,y
		auxdc = CreateCompatibleDC(hdc);
		SelectObject(auxdc, figura1);
		//BitBlt(hdc, 60, 60, 59, 59, auxdc, 0, 0, SRCCOPY);
		//StretchBlt(hdc, 0, 0, 32, 32, auxdc, 60, 60, 59, 59, SRCPAINT);

		TransparentBlt(hdc,x,y,59,59,auxdc,0,0,59,59,RGB(255,255,255));

		DeleteDC(auxdc);

		//FIM
		EndPaint(hWnd, &p);
		break;
	case WM_COMMAND:
		if (wParam == ID_FICHEIRO_REGISTAR) {
			//MessageBox(hWnd, TEXT("Frase"), TEXT("Title"), MB_OK);
			DialogBox(ghInstance, MAKEINTRESOURCE(IDD_dlgMain), hWnd, DlgBox1Proc);
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

LRESULT CALLBACK DlgBox1Proc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
	TCHAR buf[100];
	int i;
	TCHAR _i[5];
	switch (messg)
	{
		
	case WM_INITDIALOG:
		SetDlgItemText(hWnd, IDC_txtIpServ, TEXT("127.0.0.1"));
		SendDlgItemMessage(hWnd, IDC_lbxPlayers, LB_ADDSTRING, 0, TEXT("Luís"));
		return TRUE;

	case WM_COMMAND:
		if (wParam == IDC_btnCheckConn) {
			GetDlgItemText(hWnd, IDC_txtIpServ, buf, 100);
			i = InicializarPipes(&hPipe, &hPipeJogo, buf);
			if (i < 0) {
				MessageBox(hWnd, TEXT("Ligado"), TEXT("Ligação Estabelecida ao servidor!"), MB_OK);
			}
			else {
				_tcscpy(buf, TEXT("Não foi possível ligar ao servidor! Erro: "));
				_stprintf(_i, TEXT("%d"), i);
				_tcscat(buf, _i);
				MessageBox(hWnd, buf, TEXT("Não ligado"), MB_OK);
			}
			return TRUE;
		}
		if (wParam == IDOK) {
			GetDlgItemText(GetModuleHandle(NULL), IDC_txtLogin, buf, 100);
			MessageBox(GetModuleHandle(NULL), buf, TEXT("Lido"), MB_OK);
			return TRUE;
		}

		if (LOWORD(wParam) == IDOK && HIWORD(wParam) == LBN_DBLCLK) {
			//Buscar Item Selecionado
			i = SendDlgItemMessage(hWnd, IDC_lbxPlayers, LB_GETCURSEL, 0, 0);
		}
	
		if (wParam == IDCANCEL) {
			EndDialog(hWnd, 0);
		}
		return TRUE;
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	}
	return FALSE;
}


