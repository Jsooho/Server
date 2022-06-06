#include "HThread.h"

bool HThread::ThreadCreate()
{
	if (m_bStart == true)
	{
		return false;
	}
	else
	{
		m_Thread = _beginthreadex(nullptr, 0, Running, this, 0, &m_iId);
		m_bStart = true;
	}
    return true;
}
bool HThread::Run()
{
	return true;
}
HThread::HThread()
{
	m_bStart = false;
}
HThread::~HThread()
{
	CloseHandle((HANDLE)m_Thread);
}
unsigned int __stdcall HThread::Running(LPVOID param)
{
	HThread* pThread = (HThread*)param;
	if (pThread != nullptr)
	{
		pThread->Run();
		return 1;
	}
	return 0;
}
