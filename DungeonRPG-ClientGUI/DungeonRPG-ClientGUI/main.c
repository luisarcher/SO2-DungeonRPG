#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include "lib\ClientDLL.h"

LRESULT CALLBACK TrataEventos(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DlgBox1Proc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);

//Nome da classe da janela
TCHAR *szProgName = TEXT("Base");

HINSTANCE ghInstance;

//Handlers para pipes
HANDLE hPipe;
HANDLE hPipeJogo;

// ============================================================================
// FUNÇÂO DE INÍCIO DO PROGRAMA: WinMain()
// ============================================================================
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
		LPSTR lpCmdLine, int nCmdShow) {

	HWND hWnd;			// hWnd é o handler da janela, gerado mais abaixo 
						// por CreateWindow()
	MSG lpMsg;			// MSG é uma estrutura definida no Windows para as mensagens
	WNDCLASSEX wcApp;	// WNDCLASSEX é uma estrutura cujos membros servem para 
						// definir as características da classe da janela
	ghInstance = hInst;
						// ============================================================================
						// 1. Definição das características da janela "wcApp" 
						//    (Valores dos elementos da estrutura "wcApp" do tipo WNDCLASSEX)
						// ============================================================================

	wcApp.cbSize = sizeof(WNDCLASSEX);	// Tamanho da estrutura WNDCLASSEX
	wcApp.hInstance = hInst;			// Instância da janela actualmente exibida 
										// ("hInst" é parâmetro de WinMain e vem 
										// inicializada daí)
	wcApp.lpszClassName = szProgName;	// Nome da janela (neste caso = nome do programa)
	wcApp.lpfnWndProc = TrataEventos;		// Endereço da função de processamento da janela 
											// ("WndProc" foi declarada no início e encontra-se
											// mais abaixo)
	wcApp.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;			// Estilo da janela: Fazer o redraw
																// se for modificada horizontal ou
																// verticalmente
																/*OUTROS ESTILOS POSSíVEIS: CS_DBLCLKS			permite a captura de duplo cliques do rato
																CS_NOCLOSE			retira o 'X' na barra para fechar a aplicação*/

	wcApp.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// "hIcon" = handler do ícon normal
													// "NULL" = Icon definido no Windows
													// "IDI_AP..." Ícone "aplicação"
	wcApp.hIconSm = LoadIcon(NULL, IDI_INFORMATION);	// "hIcon" = handler do ícon pequeno
														// "NULL" = Icon definido no Windows
														// "IDI_WIN..." Ícon "Wind.NT logo"
														/*OUTROS TIPOS DE ICONES:	IDI_ASTERISK		Same as IDI_INFORMATION.
														IDI_ERROR			Hand-shaped icon.
														IDI_EXCLAMATION		Same as IDI_WARNING.
														IDI_HAND			Same as IDI_ERROR.
														IDI_INFORMATION		Asterisk icon.
														IDI_QUESTION		Question mark icon.
														IDI_WARNING			Exclamation point icon*/

	wcApp.hCursor = LoadCursor(NULL, IDC_ARROW);	// "hCursor" = handler do cursor (rato)
													// "NULL" = Forma definida no Windows
													// "IDC_ARROW" Aspecto "seta" 
													/*OUTROS TIPOS DE CURSORES:	IDC_CROSS IDC_HAND IDC_HELP IDC_UPARROW IDC_WAIT */



	wcApp.lpszMenuName = IDR_MENU1;						// Classe do menu que a janela pode ter
													// (NULL = não tem menu)
	wcApp.cbClsExtra = 0;							// Livre, para uso particular
	wcApp.cbWndExtra = 0;							// Livre, para uso particular

	wcApp.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // "hbrBackground" = handler para 
															   // "brush" de pintura do fundo da
															   // janela. Devolvido por
															   // "GetStockObject". Neste caso o
															   // fundo vai ser branco
															   /*OUTRAS CORES DE BRUSH:	BLACK_BRUSH  DKGRAY_BRUSH GRAY_BRUSH LTGRAY_BRUSH  */

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
		TEXT("Exemplo de Janela Principal em C"),	// Texto que figura na barra da janela
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
	ShowWindow(hWnd, nCmdShow);	// "hWnd"= handler da janela, devolvido 
								// por "CreateWindow"; "nCmdShow"= modo de
								// exibição (p.e. normal, modal); é passado
								// como parâmetro de WinMain()

	UpdateWindow(hWnd);			// Refrescar a janela (Windows envia à janela
								// uma mensagem para pintar, mostrar dados,
								// (refrescar), etc)

								// ============================================================================
								// 5. Loop de Mensagens
								// ============================================================================
								// O Windows envia mensagens às janelas (programas). Estas mensagens ficam numa
								// fila de espera até que GetMessage(...) possa ler "a mensagem seguinte"	

								// Parâmetros de "getMessage":
								//  1)	"&lpMsg"=Endereço de uma estrutura do tipo MSG ("MSG lpMsg" ja foi 
								//		declarada no início de WinMain()):
								/*			HWND hwnd		handler da janela a que se destina a mensagem
								UINT message	Identificador da mensagem
								WPARAM wParam	Parâmetro, p.e. código da tecla premida
								LPARAM lParam	Parâmetro, p.e. se ALT também estava premida
								DWORD time		Hora a que a mensagem foi enviada pelo Windows
								POINT pt		Localização do mouse (x, y)
								2)   handle da window para a qual se pretendem receber mensagens
								(=NULL se se pretendem receber as mensagens para todas as janelas
								pertencentes ao thread actual)
								3)	 Código limite inferior das mensganes que se pretendem receber
								4)   Código limite superior das mensagens que se pretendem receber
								*/

								// NOTA: GetMessage() devolve 0 quando for recebida a mensagem de fecho da janela,
								// 	     terminando então o loop de recepção de mensagens, e o programa 


	while (GetMessage(&lpMsg, NULL, 0, 0)) {
		TranslateMessage(&lpMsg);			// Pré-processamento da mensagem
											// p.e. obter código ASCII da tecla
											// premida
		DispatchMessage(&lpMsg);			// Enviar a mensagem traduzida de volta
											// ao Windows, que aguarda até que a 
											// possa reenviar à função de tratamento
											// da janela, CALLBACK TrataEventos (mais 
											// abaixo)
	}
	return((int)lpMsg.wParam);		// Retorna-se sempre o parâmetro "wParam" da
									// estrutura "lpMsg"
}


// ============================================================================
// FUNÇÂO DE PROCESSAMENTO DA JANELA
// Esta função pode ter um nome qualquer: Apenas é necesário que na 
// inicialização da estrutura "wcApp", feita no início de WinMain(), se 
// identifique essa função. Neste caso "wcApp.lpfnWndProc = WndProc"
//
// WndProc recebe as mensagens enviadas pelo Windows (depois de lidas e pré-
// processadas no loop "while" da função WinMain()
//
// Parâmetros:
//		hWnd	O handler da janela, obtido no CreateWindow()
//		messg	Ponteiro para a estrutura mensagem (ver estrutura em 5. Loop...
//		wParam	O parâmetro wParam da estrutura messg (a mensagem)
//		lParam	O parâmetro lParam desta mesma estrutura
//
// NOTA: Estes parâmetros aparecem aqui directamente acessíveis o que 
//		 simplifica o acesso aos seus valores
//
// A função EndProc é sempre do tipo "switch..." com "cases" que descriminam
// a mensagem recebida e a tratar. Estas mensagens são identificadas por macros
// (p.e. WM_DESTROY, WM_CHAR, WM_KEYDOWN, WM_PAINT...) definidas em windows.h
// (cada WM_... corresponde a um número)
// ============================================================================
LRESULT CALLBACK TrataEventos(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc; //handle for device context
	static int x = 0, y = 0;
	static int xi = 0, xf = 0, yi = 0, yf = 0;
	static TCHAR letra[200] = TEXT("texto");
	TCHAR pal[100];
	PAINTSTRUCT p;
	switch (messg) {
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &p);
		TextOut(hdc, x, y, letra, _tcslen(letra));
		hdc = GetDC(hWnd);
		Ellipse(hdc, xi, yi, xf, yf);
		//ReleaseDC(hWnd,hdc);
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
			y++;
			InvalidateRect(hWnd, NULL, 1);
			break;
		case VK_UP:
			y--;
			InvalidateRect(hWnd, NULL, 1);
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