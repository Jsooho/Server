#pragma once
#include"Std.h"
class HThread
{
public:
	uintptr_t		m_Thread;
	unsigned int	m_iId;
	bool			m_bStart;
public:
	bool ThreadCreate();
	static unsigned int WINAPI Running(LPVOID obj);
	virtual bool Run();
public:
	HThread();
	~HThread();
};

