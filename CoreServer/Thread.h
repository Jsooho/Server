#pragma once
#include <process.h>
#include "ServerObj.h"
class Thread : public ServerObj
{
public:
	uintptr_t		m_Thread;
	unsigned int	m_iId;
	LPVOID			m_pObj;
	bool			m_bStart;
public:
	void Create();
	void Create(LPVOID pValue);
	virtual bool Run();
	void Join();
	void Detach();
	static unsigned int WINAPI Running(LPVOID obj);
public:
	Thread();
	Thread(LPVOID obj);
	virtual ~Thread();

};

