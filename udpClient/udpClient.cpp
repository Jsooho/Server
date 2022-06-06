#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

void main()
{
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		return;
	}
	SOCKET sock = socket(AF_INET,
		SOCK_DGRAM,
		0);
	if (sock == INVALID_SOCKET)
	{
		return;
	}

	SOCKADDR_IN serveradd;
	ZeroMemory(&serveradd, sizeof(serveradd));
	serveradd.sin_family = AF_INET;
	serveradd.sin_port = htons(9199);
	serveradd.sin_addr.s_addr =
		inet_addr("127.0.0.1");

	//bind(sock, (SOCKADDR*)&serveradd, sizeof(serveradd));

	SOCKADDR_IN clientAddr;
	char buf[256] = { 0, };
	char recvbuf[256] = { 0, };
	int addlen;
	int iCount = 0;
	while (1)
	{
		sprintf(buf, "%s:%d", "sooho", iCount++);
		int iRet = sendto(sock, buf, strlen(buf),
			0,
			(SOCKADDR*)&serveradd,
			sizeof(serveradd));
		if (iRet == SOCKET_ERROR)
		{
			return;
		}
		addlen = sizeof(clientAddr);
		iRet = recvfrom(sock, recvbuf, 256, 0,
			(SOCKADDR*)&clientAddr,
			&addlen);
		if (iRet == SOCKET_ERROR)
		{
			return;
		}
		recvbuf[iRet] = 0;

		printf("\n[%s:%d]:%s%s",
			inet_ntoa(clientAddr.sin_addr),
			ntohs(clientAddr.sin_port),
			recvbuf, recvbuf);

		Sleep(1000);
	}

	WSACleanup();
}