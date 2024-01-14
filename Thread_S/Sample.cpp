#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "NetUser.h"
list<NetUser> g_Userlist;
CRITICAL_SECTION g_CS;
HANDLE g_hMutex;
int SendMsg(SOCKET sock ,char* msg, WORD type)
{
	// 패킷 생성
	UPACKET packet;
	ZeroMemory(&packet, sizeof(packet));
	packet.ph.len = strlen(msg) + PACKET_HEADER_SIZE; //패킷 헤더 사이즈를 더한다
	packet.ph.type = type; // 패킷 타입 지정
	memcpy(packet.msg, msg, strlen(msg)); //패킷 메시지 복사
	char* pMsg = (char*)&packet;
	int iSendSize = 0;
	do {
		int iSendByte = send(sock, &pMsg[iSendSize],
			packet.ph.len - iSendSize, 0);
		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		iSendSize += iSendByte;
	} while (iSendSize < packet.ph.len);
	return iSendSize;
}
int SendMsg(SOCKET sock, UPACKET& packet)
{
	char* pMsg = (char*)&packet;
	int iSendSize = 0;
	do {
		int iSendByte = send(sock, &pMsg[iSendSize],
			packet.ph.len - iSendSize, 0);
		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		iSendSize += iSendByte;
	} while (iSendSize < packet.ph.len);
	return iSendSize;
}
int Broadcast(NetUser& user)
{
	if (user.m_packetPool.size() > 0)
	{
		std::list<Packet>::iterator iter;
		for (iter = user.m_packetPool.begin();
			iter != user.m_packetPool.end(); )
		{
			for (NetUser& senduser : g_Userlist)
			{
				int iRet = SendMsg(senduser.m_Sock, (*iter).m_uPacket);
				if (iRet <= 0)
				{
					senduser.m_bConnect = false;
				}
			}
			iter = user.m_packetPool.erase(iter);
		}
	}
	return 1;
}
int RecvUser(NetUser& user)
{
	char RecvBuffer[1024] = { 0, };
	int RecvByte = recv(user.m_Sock, RecvBuffer, 1024, 0);
	if (RecvByte == 0)
	{
		return 0;
	}
	if (RecvByte == SOCKET_ERROR)
	{
		int iError = WSAGetLastError();
		if (iError != WSAEWOULDBLOCK)
		{
			return -1;
		}
		return 1;
	}
	user.DispatchRead(RecvBuffer, RecvByte);
	return 1;
}
DWORD WINAPI RecvThread(LPVOID lParam)
{
	//EnterCriticalSection(&g_CS);
	SOCKET RSock = (SOCKET)lParam;
	while (1)
	{
		WaitForSingleObject(g_hMutex, INFINITE);
		list<NetUser>::iterator Useriter;
		for (Useriter = g_Userlist.begin(); Useriter != g_Userlist.end();)
		{
			int recvRet = RecvUser(*Useriter);
			if (recvRet <= 0)
			{
				Useriter = g_Userlist.erase(Useriter);
			}
			else
			{
				Useriter++;
			}
		}
		//LeaveCriticalSection(&g_CS);
		ReleaseMutex(g_hMutex);
		Sleep(1);
	}
	return 0;
}
DWORD WINAPI SendThread(LPVOID lParam)
{
	//EnterCriticalSection(&g_CS);
	SOCKET Ssock = (SOCKET)lParam;
	while (1)
	{
		WaitForSingleObject(g_hMutex, INFINITE);
		std::list<NetUser>::iterator userIter;
		for (userIter = g_Userlist.begin();
			userIter != g_Userlist.end();)
		{
			int iRet = Broadcast(*userIter);
			if (iRet <= 0)
			{
				userIter = g_Userlist.erase(userIter);
			}
			else
			{
				userIter++;
			}
		}
		//LeaveCriticalSection(&g_CS);
		ReleaseMutex(g_hMutex);
		Sleep(1);
	}
	return 0;
}
int main()
{
	//InitializeCriticalSection(&g_CS);
	g_hMutex = CreateMutex(NULL, FALSE, NULL);

	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN addr;
	
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "wsa error" << endl;
		return -1;
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == SOCKET_ERROR)
	{
		cout << "sock error" << endl;
		return -1;
	}

	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9190);

	if (bind(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		cout << "bind error" << endl;
		return -1;
	}
	
	if (listen(sock, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "listen error" << endl;
		return -1;
	}

	cout << "서버 가동.........." << endl;

	u_long on = 1;
	ioctlsocket(sock, FIONBIO, &on);

	DWORD ThreadIdRecv;
	HANDLE hThreadRecv = CreateThread(0, 0, RecvThread, (LPVOID)sock, 0, &ThreadIdRecv); // 3번째 인자는 스레드의 시작함수를 지정한다
	CloseHandle(hThreadRecv);

	DWORD ThreadIdSend;
	HANDLE hThreadSend = CreateThread(0, 0, SendThread, (LPVOID)sock, 0, &ThreadIdSend);
	CloseHandle(hThreadSend);


	while (1)
	{
		SOCKET clientSock;
		SOCKADDR_IN Caddr;
		int iCsize = sizeof(Caddr);

		clientSock = accept(sock, (SOCKADDR*)&Caddr, &iCsize);
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
			NetUser user;
			user.set(clientSock, Caddr);
			//EnterCriticalSection(&g_CS);
			WaitForSingleObject(g_hMutex, INFINITE);
			g_Userlist.push_back(user);
			//LeaveCriticalSection(&g_CS);
			ReleaseMutex(g_hMutex);

			std::cout << "ip =" << inet_ntoa(Caddr.sin_addr) << "port =" << ntohs(Caddr.sin_port) << "  " << std::endl;
			u_long on = 1;
			ioctlsocket(clientSock, FIONBIO, &on);
			std::cout << g_Userlist.size() << " 명 접속중.." << std::endl;
		}
		Sleep(1);
	}
	closesocket(sock);
	WSACleanup();

	//DeleteCriticalSection(&g_CS);
	CloseHandle(g_hMutex);
}
	