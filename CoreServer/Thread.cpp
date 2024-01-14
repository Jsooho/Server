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

	// ������ ���� ������ ���ÿ� usage count�� 2�� �ȴ�. 
	// �ϳ��� ������ ����� �����ϰ�, ������ �ϳ��� ������ �ڵ��� ���ڷ� CloseHandle �Լ��� ȣ��� �� ����
	// ������ ������ ��ȯ�� �ڵ鰪�� ���ڷ� �����ϸ鼭 CloseHandle �Լ��� ��ٰ� ȣ���Ѵ�. �̷��� �Ǹ�, �������� Usage count�� 1�� �ǰ�, 
	// �����尡 �����԰� ���ÿ� Usage count�� 0�� �Ǿ� ��� �޸𸮸� ��ȯ�ϰ� �ȴ�.
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

