#include "MenuProc.h"
#include "GameData.h"
#include "Controller.h"

LRESULT CALLBACK DlgBoxConnectProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
	TCHAR buffer[100];
	int ret;
	switch (messg)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hWnd, IDC_txtIpServ, TEXT("127.0.0.1"));
		return TRUE;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			
			if (!connected) {
				GetDlgItemText(hWnd, IDC_txtIpServ, buffer, 100);
				ret = InicializarPipes(&hPipe, &hPipeJogo, buffer);

				if (!ret) {
					_tcscpy(buffer, TEXT("Could not connect to server! Error: \n"));
					_tcscat(buffer, errorHandlingPipe[ret*(-1)]);
					MessageBox(hWnd, buffer, TEXT("Not connected!"), MB_OK | MB_ICONERROR);
					return TRUE;
				}
				else {
					connected = 1;

					memset(buffer, TEXT('\0'), sizeof(TCHAR)*100);
					GetDlgItemText(hWnd, IDC_txtLogin, buffer, 100);
					if (buffer[0] != TEXT('\0'))
						if (!Register(buffer))
							MessageBox(hWnd, TEXT("Error registering on the server!"), TEXT("Register"), MB_OK | MB_ICONERROR);
					
					hThreadBroadcastReceiver = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LerBroadcast, (LPVOID)hPipeJogo, 0, NULL);
					EndDialog(hWnd, 0);
				}
			} else EndDialog(hWnd, 0);
			return TRUE;
		case IDCANCEL:
			EndDialog(hWnd, 0);
			return TRUE;
		}
		return TRUE;

	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	}
	return FALSE;
}