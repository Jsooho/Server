#pragma once
#include "Std.h"
#pragma comment	(lib, "Winmm.lib")
class HTimer
{
private:
	LARGE_INTEGER   m_Frequency;// ÁÖÆÄ¼ö
	LARGE_INTEGER   m_Current;
	LARGE_INTEGER   m_Frame;
	int				m_iTmpCounter;
public:
	int		m_iFPS;
	float   m_fSecondPerFrame;
	float	m_fGameTimer;
	TCHAR   m_szTimerString[MAX_PATH] = { 0, };
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	HTimer();
	virtual ~HTimer();
};

