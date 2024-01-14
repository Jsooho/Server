#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<iostream>
#include <conio.h>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main()
{
	WSADATA wsaData;
	SOCKET Sock;
	SOCKADDR_IN Sockaddr;
	int iCsize;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "WSA error!" << endl;
		return -1;
	}

	Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (Sock == SOCKET_ERROR)
	{
		cout << "socket error" << endl;
		return -1;
	}

	ZeroMemory(&Sockaddr, sizeof(Sockaddr));
	Sockaddr.sin_family = AF_INET;
	Sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	Sockaddr.sin_port = htons(9190);

	int Ret = connect(Sock, (SOCKADDR*)&Sockaddr, sizeof(Sockaddr));
	if (Ret == SOCKET_ERROR)
	{
		cout << "connect error" << endl;
		return -1;
	}
	cout << "접속완료!" << endl;

	u_long on = 1;
	ioctlsocket(Sock, FIONBIO, &on);
	
	char szBuffer[256];
	int iEnd = 0;
	while (1)
	{
		if (_kbhit() == 1)
		{
			int iVal = _getche();
			if (iVal == '\r' && strlen(szBuffer) == 0)
			{
				break;
			}
			if (iVal == '\r')
			{
				int iSendByte = send(Sock, szBuffer, iEnd, 0);
				if (iSendByte == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						std::cout << "비정상 서버종료됨.." << std::endl;
					}
				}
				ZeroMemory(szBuffer, sizeof(char) * 256);
			}
			else
			{
				szBuffer[iEnd++] = iVal;
			}
			
		}
		else
		{
			char szRecvBuffer[256] = { 0, };
			int iRecvByte = recv(Sock, szRecvBuffer, 256, 0);
			if (iRecvByte == 0)
			{
				cout << "서버 종료" << endl;
				return 0;
			}
			if (iRecvByte == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					std::cout << "비정상 서버종료됨.." << std::endl;
					break;
				}
			}
			else
			{
				std::cout << "\n" << szRecvBuffer;
				ZeroMemory(szRecvBuffer, sizeof(char) * 256);
			}
		}
	}
	closesocket(Sock);
	WSACleanup();
	return 0;
}