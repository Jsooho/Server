#include "Thread.h"

void Thread::Create()
{
	if (m_bStart == true)
	{
		return;
	}
	else
	{
		m_Thread = _beginthreadex(nullptr, 0, Running, this, 0, &m_iId);
		m_bStart = true;
	}
}

void Thread::Create(LPVOID obj)
{
	if (m_bStart == true)
	{
		return;
	}
	else
	{
		m_Thread = _beginthreadex(nullptr, 0, Running, this, 0, &m_iId);
		m_pObj = obj;
		m_bStart = true;
	}
}

bool Thread::Run()
{
	return true;
}

void Thread::Join()
{
	WaitForSingleObject((HANDLE)m_Thread, INFINITE);
}

void Thread::Detach()
{
	CloseHandle((HANDLE)m_Thread); 

	// 쓰레드 역시 생성과 동시에 usage count는 2가 된다. 
	// 하나는 쓰레드 종료시 감소하고, 나머지 하나는 쓰레드 핸들을 인자로 CloseHandle 함수가 호출될 때 감소
	// 쓰레드 생성시 반환된 핸들값을 인자로 전달하면서 CloseHandle 함수를 곧바고 호출한다. 이렇게 되면, 쓰레드의 Usage count는 1이 되고, 
	// 쓰레드가 종료함과 동시에 Usage count는 0이 되어 모든 메모리를 반환하게 된다.
}

unsigned int __stdcall Thread::Running(LPVOID param)
{
	Thread* pThread = (Thread*)param;
	if (pThread != nullptr)
	{
		pThread->Run();
		return 1;
	}
	return 0;
}

Thread::Thread()
{
	m_bStart = false;
	m_pObj	 = nullptr;
}

Thread::Thread(LPVOID obj)
{
	m_bStart = false;
	m_pObj   = nullptr;
	Create(obj);
}

Thread::~Thread()
{
	CloseHandle((HANDLE)m_Thread);
}

