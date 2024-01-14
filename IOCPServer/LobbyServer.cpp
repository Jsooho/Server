#include "LobbyServer.h"

DWORD WINAPI WorkerThread(LPVOID lpThreadParameter)
{
	LobbyServer* pServer = (LobbyServer*)lpThreadParameter;
	DWORD dwTransfer;//dwTransfer = �񵿱� �۾� ���
	ULONG_PTR KeyValue; //�Ϸ�PORT�� ���Ͽ� �۾��� ������ �ִ�,  ULONG_PTR(32,64��Ʈ �� �ᵵ��) = �ּ�
	OVERLAPPED* pOverlapped; //�񵿱� �۾��� ������ ����ü�� �Ѱ��ش�
	//INFINITE = �����Ͱ� ������ ��ȯ���ش�
	
	//�Ϸ� ť�� �����Ͱ� ������ �۾��ϰ� ����� , ť�� �����Ͱ� ������ ���
	while (1)
	{
		if (WaitForSingleObject(pServer->m_hKillEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}

		BOOL bReturn = ::GetQueuedCompletionStatus(pServer->g_hIOCP, &dwTransfer, &KeyValue, &pOverlapped, INFINITE);
		if (bReturn == TRUE)
		{
			//�۾�
			NetUser* pUser = (NetUser*)KeyValue;
			OV* pOv = (OV*)pOverlapped;

			if (bReturn == TRUE && pUser && pOv)
			{
				if (dwTransfer == 0 && pOv->type == 1000)
				{
					pUser->m_bConnect = false;
				}
				else
				{
					pUser->Dispatch(dwTransfer, pOv);
				}
			}
		}
		else
		{
			int Error = GetLastError();
			if (GetLastError() != WAIT_TIMEOUT)
			{
				::SetEvent(pServer->m_hKillEvent);
				break;
			}
		}
	}
	return 1;

}
bool LobbyServer::AddUser(SOCKET sock, SOCKADDR_IN clientAddr)
{
	NetUser* user = new NetUser;
	user->set(sock, clientAddr);

	u_long on = 1;
	ioctlsocket(sock, FIONBIO, &on);
	
	EnterCriticalSection(&m_CS);
	m_Userlist.push_back(user);
	LeaveCriticalSection(&m_CS);


	::CreateIoCompletionPort((HANDLE)sock, g_hIOCP, (ULONG_PTR)user, 0);//Ű���� ������ �����ؼ� ����Ѵ� , Ű���� �������� �ٸ��⶧���� 
	user->Recv();

	char ip[INET_ADDRSTRLEN];
	cout << "IP = " << user->m_csName << " ";
	cout << "PORT = " << (USHORT)user->m_iPort << " " << "�����մϴ�!" << endl;
	
	
	return true;
}
bool LobbyServer::Init(int iport)
{
	Server::Init(iport);

	g_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
	{
		DWORD id;
		g_hWorkThread[iThread] = ::CreateThread(0, 0, WorkerThread, this, 0, &id);
	}

	return true;
}
bool LobbyServer::Run()
{
	while (1)
	{
		EnterCriticalSection(&m_CS);
		for (NetUser* User : m_Userlist)
		{
			//ChatUser* pChat = (ChatUser*)User;
			if (User->m_packetPool.size() > 0)
			{
				Broadcast(User);
			}
		}
		std::list<NetUser*>::iterator iter;
		for (iter = m_Userlist.begin();
			iter != m_Userlist.end();)
		{
			if ((*iter)->m_bConnect == false)
			{
				(*iter)->DisConnect();
				delete (*iter);
				iter = m_Userlist.erase(iter);
			}
			else
			{
				iter++;
			}
		}

		LeaveCriticalSection(&m_CS);
		Sleep(1);
	}
	return true;
}
bool LobbyServer::Release()
{
	CloseHandle(g_hIOCP);
	Server::Release();
	return true;
}