#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<iostream>
#include <list>
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
				int iRecvByte = recv(user.m_Sock, szRecvBuffer, 256, 0);
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
				else
				{
					list<User>::iterator Senditer;
					for (Senditer = userlist.begin(); Senditer != userlist.end();)
					{
						User user = *Senditer;
						cout << szRecvBuffer << "받았음" << endl;
						int iSendByte = send(user.m_Sock, szRecvBuffer, iRecvByte, 0);
						cout << user.m_name << " : " << iSendByte << "보냈음" << endl;
						if (iSendByte == SOCKET_ERROR)
						{
							int iError = WSAGetLastError();
							if (iError != WSAEWOULDBLOCK)
							{
								closesocket(user.m_Sock);
								Senditer = userlist.erase(Senditer);
								std::cout << user.m_name << " 비정상 종료" << std::endl;
							}
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
	}
	closesocket(ServerSocket);
	WSACleanup();
	
	return 0;
}
