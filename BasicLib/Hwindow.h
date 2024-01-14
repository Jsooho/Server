#pragma once
#include"Std.h"

class Hwindow
{
	//������� = ���, �ٸ��������� ����� ��
public:
	
	HINSTANCE m_hInstance;
	HWND      m_hWnd;
	RECT	  m_rtWindow;
	RECT      m_rtClient;
	bool	  m_bGameRun =true;
public:
	//1��: �ü���� Ŭ���� ���
	//bool SetWindowClass(HINSTANCE hInstance);
	//2��: ������ ����
	//bool SetWindow(const WCHAR* szTitle = L"Sample",int iWidth = 800,int iHeight = 600);
	//3��: �޼������ν��� ó��!!
	//�޼��� ť�� ������ ���ӷ��� ���ư��� = (�̰ſ� ���� ����!!)
	bool InitWindows(HINSTANCE hInstance, int nCmdShow, const WCHAR* strWindowTitle, int iX, int iY);
	virtual LRESULT  MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool WinRun();
	virtual bool GameInit();
	virtual bool GameRun();
	virtual bool GameRelease();
	
public:
	Hwindow();
};

