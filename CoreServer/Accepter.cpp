#include "Accepter.h"
#include "Server.h"


bool Accepter::Run()
{
	Server* pServer = (Server*)m_pObj;
	SOCKET sock = pServer->m_listenSock;
	SOCKADDR_IN Caddr;
	int iCsize = sizeof(Caddr);
	while (1)
	{

		SOCKET clientSock = accept(pServer->m_listenSock, (SOCKADDR*)&Caddr, &iCsize);
		if (clientSock == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			if (iError != WSAEWOULDBLOCK)
			{
				cout << "accept error" << endl;
				break;
			}
		}
		else
		{
			pServer->AddUser(clientSock, Caddr);
			std::cout << pServer->m_Userlist.size() << " 명 접속중 입니다" << std::endl;
		}
		Sleep(1);
	}
	return true;
}

Accepter::Accepter()
{
}

Accepter::Accepter(LPVOID value) : Thread(value)
{
}