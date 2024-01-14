#pragma once
#include"Std.h"

class Hwindow
{
	//멤버변수 = 고민, 다른곳에서도 사용할 것
public:
	
	HINSTANCE m_hInstance;
	HWND      m_hWnd;
	RECT	  m_rtWindow;
	RECT      m_rtClient;
	bool	  m_bGameRun =true;
public:
	//1번: 운영체제에 클래스 등록
	//bool SetWindowClass(HINSTANCE hInstance);
	//2번: 윈도우 생성
	//bool SetWindow(const WCHAR* szTitle = L"Sample",int iWidth = 800,int iHeight = 600);
	//3번: 메세지프로시저 처리!!
	//메세지 큐가 끝나면 게임런이 돌아간다 = (이거에 대한 이해!!)
	bool InitWindows(HINSTANCE hInstance, int nCmdShow, const WCHAR* strWindowTitle, int iX, int iY);
	virtual LRESULT  MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool WinRun();
	virtual bool GameInit();
	virtual bool GameRun();
	virtual bool GameRelease();
	
public:
	Hwindow();
};

