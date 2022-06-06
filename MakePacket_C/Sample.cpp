#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <conio.h>
#include "Packet.h"
#pragma comment	(lib, "ws2_32.lib")
using namespace std;

int SendMsg(SOCKET sock, char* msg, WORD type)
{
	// 1번 패킷 생성
	UPACKET packet;
	ZeroMemory(&packet, sizeof(packet));
	packet.pheader.len = strlen(msg) + PACKET_HEADER_SIZE;
	packet.pheader.type = type;
	memcpy(packet.msg, msg, strlen(msg));
	// 2번 패킷 전송 : 운영체제 sendbuffer(short바이트), recvbuffer
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
int SendPacket(SOCKET sock, char* msg, WORD type)
{
	// 1번 패킷 생성
	Packet tPacket(type);
	tPacket << "soo" << msg;
	//Packet tPacketTest(tPacket);
	ChatMsg recvdata;
	ZeroMemory(&recvdata, sizeof(recvdata));
	tPacket >>  recvdata.name >> recvdata.message;
	char* pData = (char*)&tPacket.m_uPacket;
	int iSendSize = 0;
	do {
		int iSendByte = send(sock, &pData[iSendSize],
			tPacket.m_uPacket.pheader.len - iSendSize, 0);
		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		iSendSize += iSendByte;
	} while (iSendSize < tPacket.m_uPacket.pheader.len);
	return iSendSize;
}
int RecvPacketHeader(SOCKET sock, UPACKET& recvPacket)
{
	char szRecvBuffer[256] = { 0, };
	//패킷헤더 받기
	ZeroMemory(&recvPacket, sizeof(recvPacket));
	bool bRun = true;
	int iRecvSize = 0;
	do {
		int iRecvByte = recv(sock, szRecvBuffer,
			PACKET_HEADER_SIZE, 0);
		iRecvSize += iRecvByte;
		if (iRecvByte == 0)
		{
			closesocket(sock);
			std::cout << " 접속종료됨." << std::endl;
			return -1;
		}
		if (iRecvByte == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			if (iError != WSAEWOULDBLOCK)
			{
				std::cout << " 비정상 접속종료됨." << std::endl;
				return -1;
			}
			else
			{
				return 0;
			}
		}
	} while (iRecvSize < PACKET_HEADER_SIZE);
	memcpy(&recvPacket.pheader, szRecvBuffer, PACKET_HEADER_SIZE);
	return 1;
}
int RecvPacketData(SOCKET sock, UPACKET& recvPacket)
{
	// 데이터 받기
	int iRecvSize = 0;
	do {
		int iRecvByte = recv(sock, recvPacket.msg,
			recvPacket.pheader.len - PACKET_HEADER_SIZE - iRecvSize, 0);
		iRecvSize += iRecvByte;
		if (iRecvByte == 0)
		{
			closesocket(sock);
			std::cout << " 접속종료됨." << std::endl;
			return -1;
		}
		if (iRecvByte == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			if (iError != WSAEWOULDBLOCK)
			{
				std::cout << " 비정상 접속종료됨." << std::endl;
				return -1;
			}
			else
			{
				return 0;
			}
		}
	} while (iRecvSize < recvPacket.pheader.len - PACKET_HEADER_SIZE);
	return 1;
}


int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "wsa error" << endl;
		return -1;
	}
	SOCKET sock;
	SOCKADDR_IN addr;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == SOCKET_ERROR)
	{
		cout << "socket error" << endl;
		return -1;
	}

	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(9190);

	if (connect(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) 
	{
		cout << "connect error" << endl;
		return -1;
	}

	cout << "접속 성공했습니다!" << endl;

	u_long on = 1;
	ioctlsocket(sock, FIONBIO, &on);

	char szBuffer[256] = { 0, };
	int End = 0;
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
				// 방법 1
				//int iSendByte = SendMsg(sock, szBuffer, PACKET_CHAT_MSG);
				// 방법 2
				int iSendByte = SendPacket(sock, szBuffer, PACKET_CHAT_MSG);
				if (iSendByte < 0)
				{
					std::cout << "비정상 종료됨.." << std::endl;
					break;
				}
				End = 0;
				ZeroMemory(szBuffer, sizeof(char) * 256);
			}
			else
			{
				szBuffer[End++] = iVal;
			}

		}
		else
		{
			UPACKET packet;
			int iRet = RecvPacketHeader(sock, packet);
			if (iRet < 0) break;
			if (iRet == 1)
			{
				int iRet = RecvPacketData(sock, packet);
				if (iRet < 0)
				{
					break;
				}
				if (iRet == 0)
				{
					continue;
				}
				// 메세지 처리
				Packet data;
				data.m_uPacket = packet;
				ChatMsg recvdata;
				ZeroMemory(&recvdata, sizeof(recvdata));
				data >>  recvdata.name >>  recvdata.message;
				std::cout << "\n" <<  "[" << recvdata.name << "]" << recvdata.message;
			}
		}
	}
	std::cout << "접속종료" << std::endl;
	closesocket(sock);
	WSACleanup();
}