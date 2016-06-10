#include "MenuProc.h"

LRESULT CALLBACK DlgBoxConnectProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
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