#include "LobbyServer.h"

DWORD WINAPI WorkerThread(LPVOID lpThreadParameter)
{
	LobbyServer* pServer = (LobbyServer*)lpThreadParameter;
	DWORD dwTransfer;//dwTransfer = 비동기 작업 결과
	ULONG_PTR KeyValue; //완료PORT를 통하여 작업을 나눌수 있다,  ULONG_PTR(32,64비트 다 써도됨) = 주소
	OVERLAPPED* pOverlapped; //비동기 작업한 오버렙 구조체도 넘겨준다
	//INFINITE = 데이터가 있으면 반환해준다
	
	//완료 큐에 데이터가 있으면 작업하게 만든다 , 큐에 데이터가 없으면 대기
	while (1)
	{
		if (WaitForSingleObject(pServer->m_hKillEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}

		BOOL bReturn = ::GetQueuedCompletionStatus(pServer->g_hIOCP, &dwTransfer, &KeyValue, &pOverlapped, INFINITE);
		if (bReturn == TRUE)
		{
			//작업
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


	::CreateIoCompletionPort((HANDLE)sock, g_hIOCP, (ULONG_PTR)user, 0);//키값을 유저로 변경해서 사용한다 , 키값이 유저마다 다르기때문에 
	user->Recv();

	char ip[INET_ADDRSTRLEN];
	cout << "IP = " << user->m_csName << " ";
	cout << "PORT = " << (USHORT)user->m_iPort << " " << "접속합니다!" << endl;
	
	
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