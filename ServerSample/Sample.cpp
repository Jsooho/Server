#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include"Server.h"

DWORD WINAPI RecvThread(LPVOID lParam)
{
	
	Server* pServer = (Server*)lParam;
	SOCKET RSock = pServer->m_listenSock;
	while (1)
	{
		EnterCriticalSection(&pServer->m_CS);
		//WaitForSingleObject(pServer->m_hMutex, INFINITE);
		list<NetUser>::iterator Useriter;
		for (Useriter = pServer->m_Userlist.begin(); Useriter != pServer->m_Userlist.end();)
		{
			int recvRet = pServer->RecvUser(*Useriter);
			if (recvRet <= 0)
			{
				Useriter = pServer->m_Userlist.erase(Useriter);
			}
			else
			{
				Useriter++;
			}
		}
		LeaveCriticalSection(&pServer->m_CS);
		//ReleaseMutex(pServer->m_hMutex);
		Sleep(1);
	}
	return 0;
}
DWORD WINAPI SendThread(LPVOID lParam)
{
	
	Server* pServer = (Server*)lParam;
	SOCKET Ssock = pServer->m_listenSock;
	while (1)
	{
		EnterCriticalSection(&pServer->m_CS);
		//WaitForSingleObject(pServer->m_hMutex, INFINITE);
		std::list<NetUser>::iterator userIter;
		for (userIter = pServer->m_Userlist.begin();
			userIter != pServer->m_Userlist.end();)
		{
			int iRet = pServer->Broadcast(*userIter);
			if (iRet <= 0)
			{
				userIter = pServer->m_Userlist.erase(userIter);
			}
			else
			{
				userIter++;
			}
		}
		LeaveCriticalSection(&pServer->m_CS);
		//ReleaseMutex(pServer->m_hMutex);
		Sleep(1);
	}
	return 0;
}

void main()
{
	Server server;
	server.Init();
	
	DWORD ThreadIdRecv;
	HANDLE hThreadRecv = CreateThread(0, 0, RecvThread, (LPVOID)&server, 0, &ThreadIdRecv); // 3번째 인자는 스레드의 시작함수를 지정한다
	DWORD ThreadIdSend;
	HANDLE hThreadSend = CreateThread(0, 0, SendThread, (LPVOID)&server, 0, &ThreadIdSend);

	server.Run();
	server.Release();
	CloseHandle(hThreadRecv);
	CloseHandle(hThreadSend);
}