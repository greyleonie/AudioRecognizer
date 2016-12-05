////////////////////////////////////////////////////////
//Bank.h
#ifndef __BANK_H__
#define	__BANK_H__

int PASCAL WinMain(	HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdParam, int nCmdShow);
LRESULT CALLBACK  WndProc (HWND hwnd, UINT message, UINT wParam, LONG lParam);
VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime);

#endif//__BANK_H__
