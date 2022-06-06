#pragma once
#include"Std.h"
class Hwindow
{
public:
	HWND m_hWnd;
	HINSTANCE m_hInstance;

public:
	bool InitWindow(HINSTANCE hInstance, int nCmdShow, const WCHAR* strwindowtitle);
};

