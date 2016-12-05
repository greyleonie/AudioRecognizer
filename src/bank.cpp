//////////////////////////////////////////////////
//Bank.cpp
//#include "include/stdafx.h"
#include <windows.h>
#include <commdlg.h>
#include <commctrl.h>
#include <windowsx.h>

#include "resource/resource.h"
#include "include/bank.h"
#include "include/d.h"
#include "include/tc08a32.h"
#include "include/NewSig.h"
BOOL bHaveLoadDriver;
HINSTANCE hInst;
HINSTANCE hLib;
extern WORD TotalLine;
HWND hGWnd;

int PASCAL WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdParam, int nCmdShow)
{
	static char szAppName[] = "Bank";
	HWND        hwnd;
	MSG         msg;
	WNDCLASS    wndclass;
	char		strSys[200];


//	Unused(lpszCmdParam); 

	if (!hPrevInstance)
	{
		wndclass.style         = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc   = (WNDPROC)WndProc;
		wndclass.cbClsExtra    = 0 ;
		wndclass.cbWndExtra    = DLGWINDOWEXTRA;
		wndclass.hInstance     = hInstance;
		wndclass.hIcon         = LoadIcon (hInstance, "IDI_GZSWDX");
		wndclass.hCursor       = LoadCursor (NULL, IDI_APPLICATION);
		wndclass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
		wndclass.lpszMenuName  = NULL;
		wndclass.lpszClassName = szAppName;

		ATOM a=RegisterClass (&wndclass);
	}

	GetSystemDirectory(strSys,200);
	strcat(strSys,"\\comctl32.dll");
	hLib = LoadLibrary(strSys);

	hInst = hInstance;
	hwnd = CreateDialog(hInstance, "GZSWDX", 0, NULL);
	hGWnd=hwnd;
	
	ShowWindow (hwnd, nCmdShow);
	UpdateWindow(hwnd);
	
	bHaveLoadDriver=yzInitSystem();
	if(!bHaveLoadDriver) PostQuitMessage(0);

	//FARPROC lpProc;
	//lpProc = MakeProcInstance((FARPROC)TimerProc, hInstance);
	SetTimer(hwnd,ID_TIME,100,(TIMERPROC)TimerProc); 

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam ;
}

VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime)
{ 
	if(idEvent!=ID_TIME) return;
	yzDoWork();
}

LRESULT CALLBACK  WndProc (HWND hwnd, UINT message, UINT wParam, LONG lParam)
{

	switch (message)
	{
	 	case WM_DESTROY :
			KillTimer(hwnd,ID_TIME);
			if(bHaveLoadDriver) yzExitSystem();
			PostQuitMessage (0) ;
			return 0 ;
			break;
		case WM_COMMAND:
			if(LOWORD(wParam)==ID_OK)
			{
				KillTimer(hwnd,ID_TIME);
				if(bHaveLoadDriver) yzExitSystem();
				PostQuitMessage(0);
			}
			return 0;
			break;
		default:
			return DefWindowProc (hwnd, message, wParam, lParam) ;
			break;
	}
}



