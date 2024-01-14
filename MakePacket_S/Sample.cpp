#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<iostream>
#include <list>
#include "Packet.h"
#pragma comment (lib, "ws2_32.lib")
using namespace std;

struct User
{
	SOCKET m_Sock;
	SOCKADDR_IN m_Addr;
	int m_iPort;
	char* m_name;
	void Set(SOCKET sock, SOCKADDR_IN addr)
	{
		m_Sock = sock;
		m_Addr = addr;
		m_name = inet_ntoa(addr.sin_addr);
		m_iPort = ntohs(addr.sin_port);
	}
};
int SendMsg(SOCKET sock, char* msg, WORD type)
{
	//패킷 생성하기
	UPACKET packet;
	ZeroMemory(&packet, sizeof(packet));
	packet.pheader.len = strlen(msg) + PACKET_HEADER_SIZE;
	packet.pheader.type = type;
	memcpy(packet.msg, msg, strlen(msg));
	//패킷 전송
	char* pMsg = (char*)&packet;
	int iSendSize = 0;
	do 
	{
		int iSendByte = send(sock, &pMsg[iSendSize], packet.pheader.len - iSendSize, 0);
		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		iSendSize += iSendByte;
	} while (iSendSize < packet.pheader.len);
	return iSendSize;
}
int SendMsg(SOCKET sock, UPACKET& packet)
{
	char* pMsg = (char*)&packet;
	int iSendSize = 0;
	do {
		int iSendByte = send(sock, &pMsg[iSendSize],
			packet.pheader.len - iSendSize, 0);
		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		iSendSize += iSendByte;
	} while (iSendSize < packet.pheader.len);
	return iSendSize;
}
int main()
{
	WSADATA wsaData;
	SOCKET ServerSocket;
	SOCKET ClientSocket;
	SOCKADDR_IN Saddr;
	SOCKADDR_IN Caddr;
	int iCsize;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "WSA error!" << endl;
		return -1;
	}

	ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ServerSocket == SOCKET_ERROR)
	{
		cout << "socket error" << endl;
		return -1;
	}

	ZeroMemory(&Saddr, sizeof(Saddr));
	Saddr.sin_family = AF_INET;
	Saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	Saddr.sin_port = htons(9190);

	if (bind(ServerSocket, (SOCKADDR*)&Saddr, sizeof(Saddr)) == SOCKET_ERROR)
	{
		cout << "bind error" << endl;
		return -1;
	}

	if (listen(ServerSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "listen error" << endl;
		return -1;
	}

	cout << "서버작동 시작!" << endl;

	u_long on = 1;
	ioctlsocket(ServerSocket, FIONBIO, &on);

	list<User> userlist;

	while (1)
	{
		iCsize = sizeof(Caddr);
		ClientSocket = accept(ServerSocket, (SOCKADDR*)&Caddr, &iCsize);
		if (ClientSocket == SOCKET_ERROR)
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
			User user;
			user.Set(ClientSocket, Caddr);
			userlist.push_back(user);
			cout << "ip = " << inet_ntoa(Caddr.sin_addr) << "port =" << ntohs(Caddr.sin_port) << " " << endl;
			u_long on = 1; //1 이면 on, 0이면 off
			ioctlsocket(ClientSocket, FIONBIO, &on);
		}
		if (userlist.size() > 0)
		{
			list<User>::iterator iter;
			for (iter = userlist.begin(); iter != userlist.end();)
			{
				User user = *iter;
				char szRecvBuffer[256] = { 0, };
				//패킷헤더 받기
				UPACKET recvPacket;
				ZeroMemory(&recvPacket, sizeof(recvPacket));
				int iRecvSize = 0;
				do {
					int iRecvByte = recv(user.m_Sock, szRecvBuffer, PACKET_HEADER_SIZE, 0);
					iRecvSize += iRecvByte;
					if (iRecvByte == 0)
					{
						closesocket(user.m_Sock);
						userlist.erase(iter);
						cout << user.m_name << "접속 종료!" << endl;
						continue;
					}
					if (iRecvByte == SOCKET_ERROR)
					{
						int iError = WSAGetLastError();
						if (iError != WSAEWOULDBLOCK)
						{
							userlist.erase(iter);
							std::cout << user.m_name << "비정상 종료" << std::endl;
							break;
						}
						else
						{
							break;
						}
					}
				} while (iRecvSize < PACKET_HEADER_SIZE);

				if (iRecvSize == SOCKET_ERROR)
				{
					if (iter != userlist.end())
					{
						iter++;
					}
					continue;
				}

				memcpy(&recvPacket.pheader, szRecvBuffer, PACKET_HEADER_SIZE);

				//데이터 받기
				iRecvSize = 0;
				do {
					int iRecvByte = recv(user.m_Sock, recvPacket.msg , recvPacket.pheader.len - PACKET_HEADER_SIZE - iRecvSize, 0);
					iRecvSize += iRecvByte;
					if (iRecvByte == 0)
					{
						closesocket(user.m_Sock);
						userlist.erase(iter);
						cout << user.m_name << "접속 종료!" << endl;
						continue;
					}
					if (iRecvByte == SOCKET_ERROR)
					{
						int iError = WSAGetLastError();
						if (iError != WSAEWOULDBLOCK)
						{
							userlist.erase(iter);
							std::cout << user.m_name << "비정상 종료" << std::endl;
						}
						else
						{
							iter++;
						}
					}
				} while (iRecvSize < recvPacket.pheader.len - PACKET_HEADER_SIZE);

				Packet data;
				data.m_uPacket = recvPacket;
				ChatMsg recvdata;
				ZeroMemory(&recvdata, sizeof(recvdata));
				data >> recvdata.name >> recvdata.message;
				cout << "[" << recvdata.name << "]" << recvdata.message;

				list<User>::iterator Senditer;
				for (Senditer = userlist.begin(); Senditer != userlist.end();)
				{
					User user = *Senditer;
					int iSendSize = SendMsg(user.m_Sock, recvPacket);
					if (iSendSize < 0)
					{
						closesocket(user.m_Sock);
						Senditer = userlist.erase(Senditer);
						cout << user.m_name <<"비정상 종료" << endl;
					}
					else
					{
						Senditer++;
					}
				}
				if (iter != userlist.end())
				{
					iter++;
				}
			}
		}
	}
	closesocket(ServerSocket);
	WSACleanup();

	return 0;
}
