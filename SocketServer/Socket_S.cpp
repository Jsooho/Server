#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<iostream>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

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

	while (1)
	{
		iCsize = sizeof(Caddr);
		ClientSocket = accept(ServerSocket, (SOCKADDR*)&Caddr, &iCsize);
		if (ClientSocket == SOCKET_ERROR)
		{
			cout << "accept error" << endl;
			return -1;
		}

		cout << "ip = " << inet_ntoa(Caddr.sin_addr) << "port =" << ntohs(Caddr.sin_port) << " " << endl;

		u_long on = 1; //1 이면 on, 0이면 off
		ioctlsocket(ClientSocket, FIONBIO, &on);

		while (1)
		{
			char szRecvBuffer[256] = { 0, };
			int iRecvByte = recv(ClientSocket, szRecvBuffer, 256, 0);
			if (iRecvByte == 0)
			{
				cout << "ip = " << inet_ntoa(Caddr.sin_addr) << "port =" << ntohs(Caddr.sin_port) << " " << endl;
				cout << "종료했습니다!" << endl;
				break;
			}
			if (iRecvByte == SOCKET_ERROR)
			{
				int iError = WSAGetLastError();
				if (iError != WSAEWOULDBLOCK)
				{
					break;
				}
				
			}
			else
			{
				cout << szRecvBuffer << "받음" << endl;
				int iSendByte = send(ClientSocket, szRecvBuffer, iRecvByte, 0);
				cout << iSendByte << "보냈음" << endl;
			}
			
		}
		closesocket(ClientSocket);
	}
	
	WSACleanup();
	return 0;
}