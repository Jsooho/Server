#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

void main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return;
	}
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

	SOCKADDR_IN uaddr;
	ZeroMemory(&uaddr, sizeof(uaddr));

	uaddr.sin_family = AF_INET;
	uaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	uaddr.sin_port = htons(9199);

	if (bind(sock, (SOCKADDR*)&uaddr, sizeof(uaddr)) == SOCKET_ERROR)
	{
		return;
	}
	while (1)
	{
		SOCKADDR_IN caddr;
		int caddrlen;
		caddrlen = sizeof(caddr);
		char buf[256] = { 0, };
		int recvsize = recvfrom(sock, buf, 256, 0, (SOCKADDR*)&caddr, &caddrlen);

		if (recvsize < 0)
		{
			continue;
		}
		printf("Handling client = %s\n %d Byte = %s", inet_ntoa(caddr.sin_addr), recvsize, buf);

		if (sendto(sock, buf, recvsize, 0, (SOCKADDR*)&caddr, sizeof(caddr)) != recvsize)
		{
			break;
		}

	}
	WSACleanup();
}