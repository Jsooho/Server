#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<iostream>
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

	int iCount = 5;

	while (iCount-- > 0)
	{
		char szBuffer[] = "hiyo";

		int iSendByte = send(Sock, szBuffer, sizeof(szBuffer), 0);
		if (iSendByte == SOCKET_ERROR)
		{
			cout << "send error" << endl;
			return -1;
		}

		char szRecvBuffer[256] = { 0, };
		int iRecvByte = recv(Sock, szRecvBuffer, 256, 0);
		if (iRecvByte == 0)
		{
			cout << "서버 종료" << endl;
			return 0;
		}
		if (iRecvByte == SOCKET_ERROR)
		{
			cout << "Client Recv error" << endl;
			return -1;
		}
		Sleep(1000);
	}
	closesocket(Sock);
	WSACleanup();
	return 0;
}