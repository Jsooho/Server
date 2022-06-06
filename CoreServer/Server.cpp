#include "Server.h"

int Server::SendMsg(SOCKET sock, char* msg, WORD type)
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
int Server::SendMsg(SOCKET sock, UPACKET& packet)
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
int Server::Broadcast(NetUser* user) 
//하나의 트래픽이 다수의 Host에게 발송할 때 하나의 트래픽으로 보낸다. 
//그리하여 회선의 부담을 주지 않는다.
//그러나 네트워크 영역에서 다수의 Host들은 무조건 트래픽을 전달 받게 된다.
//No Receiver 입장에서는 그 트래픽이 불필요할 것이다.

{
	if (user->m_packetPool.size() > 0)
	{
		std::list<Packet>::iterator iter;
		for (iter = user->m_packetPool.begin();
			iter != user->m_packetPool.end(); )
		{
			for (NetUser* senduser : m_Userlist)
			{
				int iRet = SendMsg(senduser->m_Sock, (*iter).m_uPacket);
				if (iRet <= 0)
				{
					senduser->m_bConnect = false;
				}
			}
			iter = user->m_packetPool.erase(iter);
		}
	}
	return 1;
}

bool Server::AddUser(SOCKET sock, SOCKADDR_IN clientAddr)
{
	return true;
}

bool Server::Init(int iport)
{
	WSADATA wsaData;
	SOCKADDR_IN addr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "wsa error" << endl;
		return -1;
	}

	m_listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_listenSock == SOCKET_ERROR)
	{
		cout << "sock error" << endl;
		return false;
	}

	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(iport);

	if (bind(m_listenSock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		cout << "bind error" << endl;
		return false;
	}

	if (listen(m_listenSock, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "listen error" << endl;
		return false;
	}

	std::cout << "Server Start!" << std::endl;
	Sleep(1000);
	std::cout << "Server On..." << std::endl;

	u_long on = 1;
	ioctlsocket(m_listenSock, FIONBIO, &on);

	return true;
}
bool Server::Run()
{
	return true;
}
bool Server::Release()
{
	closesocket(m_listenSock);
	WSACleanup();
	return true;
}