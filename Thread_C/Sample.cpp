#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <conio.h>
#include "Packet.h"
#pragma comment	(lib, "ws2_32.lib")

HANDLE g_hConnectEvent;
HANDLE g_hSemaphore;

int SendMsg(SOCKET sock, char* msg, WORD type)
{
	// 1번 패킷 생성
	UPACKET packet;
	ZeroMemory(&packet, sizeof(packet));
	packet.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
	packet.ph.type = type;
	memcpy(packet.msg, msg, strlen(msg));
	// 2번 패킷 전송 : 운영체제 sendbuffer(short바이트), recvbuffer
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
int SendPacket(SOCKET sock, char* msg, WORD type)
{
	// 1번 패킷 생성
	Packet tPacket(type);
	tPacket << "수호" << msg;
	TChatMsg recvdata;
	ZeroMemory(&recvdata, sizeof(recvdata));
	tPacket >>  recvdata.name >> recvdata.message;
	char* pData = (char*)&tPacket.m_uPacket;
	int iSendSize = 0;
	do {
		int iSendByte = send(sock, &pData[iSendSize],
			tPacket.m_uPacket.ph.len - iSendSize, 0);
		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		iSendSize += iSendByte;
	} while (iSendSize < tPacket.m_uPacket.ph.len);
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
	memcpy(&recvPacket.ph, szRecvBuffer, PACKET_HEADER_SIZE);
	return 1;
}
int RecvPacketData(SOCKET sock, UPACKET& recvPacket)
{
	// 데이터 받기
	int iRecvSize = 0;
	do {
		int iRecvByte = recv(sock, recvPacket.msg,
			recvPacket.ph.len - PACKET_HEADER_SIZE - iRecvSize, 0);
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
	} while (iRecvSize < recvPacket.ph.len - PACKET_HEADER_SIZE);
	return 1;
}
DWORD WINAPI SendThread(LPVOID lpThreadParameter)
{

	SOCKET sock = (SOCKET)lpThreadParameter; // 서버랑 연결되어있어서 키보드 입력받고 전달가능
	//메인스레드가 종료가 되면 추가적인 스레드도 종료됨
	//메인스레드가 종료 = 메인함수가 리턴되면 종료
	//추가적인 스레드 = 추가 스레드 함수 리턴되면 종료
	char szBuffer[256] = { 0, };

	while (1)
	{

		DWORD dwRet = WaitForSingleObject(g_hConnectEvent, INFINITE); // 뮤텍스
		if (dwRet != WAIT_OBJECT_0)
		{
			break;
		}
		ZeroMemory(szBuffer, sizeof(char) * 256);
		fgets(szBuffer, 256, stdin);//stdin = 키보드입력 (검색해서확인)

		if (strlen(szBuffer) == 0)
		{
			std::cout << "정상 종료됨.." << std::endl;
			break;
		}
		// 방법 1
		//int iSendByte = SendMsg(sock, szBuffer, PACKET_CHAT_MSG);
		// 방법 2
		int iSendByte = SendPacket(sock, szBuffer, PACKET_CHAT_MSG);
		if (iSendByte < 0)
		{
			std::cout << "비정상 종료됨.." << std::endl;
			break;
		}
	}
	return 0;
}
DWORD WINAPI RecvTread(LPVOID lpThreadParameter)
{
	SOCKET sock = (SOCKET)lpThreadParameter;

	while (1)
	{

		DWORD dwRet = WaitForSingleObject(g_hConnectEvent, INFINITE); // 뮤텍스
		if (dwRet != WAIT_OBJECT_0)
		{
			break;
		}
		
		UPACKET packet;
		int iRet = RecvPacketHeader(sock, packet);
		if (iRet < 0) continue;
		if (iRet == 1)
		{
			int iRet = RecvPacketData(sock, packet);
			if (iRet < 0) break;
			if (iRet == 0) continue;
			// 메세지 처리
			Packet data;
			data.m_uPacket = packet;
			TChatMsg recvdata;
			ZeroMemory(&recvdata, sizeof(recvdata));
			data  >> recvdata.name >> recvdata.message;
			std::cout << "\n" <<
				"[" << recvdata.name << "]"
				<< recvdata.message;
		}
	}
	return 0;
}

//멀티스레드는 시간을 쪼개서 작업을한다(운영체제가 알아서해줌) 
//ex)하나의프로세스 안에서 = 메인스레드 0.1초, 서브스레드 0.1초 이런식으로 돌아감, 상황에따라 카운터가 다름
//우선순위는 컨트롤 가능하다.
void main()
{
	g_hConnectEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); 
	
	//스레드 생성
	//1) window api = 스레드 이해하기 좋음
	DWORD threadid;// 스레드의 번호 id
	HANDLE hThread = ::CreateThread(
		0,
		0,//스택 사이즈 (기본적으로 주어짐, 추가할당 가능)
		SendThread, // 윈도우 프로시저랑 비슷함 , 반환되면 종료됨
		(LPVOID)sock, // sendthread(추가스레드)에 넘겨주는 값, 하나의 객체에 주소만 받음 
		0, //서스펜디드
		&threadid // 반환 값
	);

	DWORD threadidRecv;// 스레드의 번호 id
	HANDLE hThreadRecv = ::CreateThread(
		0,
		0,//스택 사이즈 (기본적으로 주어짐, 추가할당 가능)
		RecvTread, // 윈도우 프로시저랑 비슷함 , 반환되면 종료됨
		(LPVOID)sock, // sendthread(추가스레드)에 넘겨주는 값, 하나의 객체에 주소만 받음 
		0, //서스펜디드
		&threadidRecv // 반환 값
	);
	
	Sleep(1000);

	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(9190);
	sa.sin_addr.s_addr = inet_addr("192.168.0.44");
	int iRet = connect(sock, (sockaddr*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}
	std::cout << "접속성공!" << std::endl;

	SetEvent(g_hConnectEvent);

	u_long on = 1;
	ioctlsocket(sock, FIONBIO, &on);

	while (1)
	{
		Sleep(1);
		//ResetEvent(g_hConnectEvent);
	}
	std::cout << "접속종료" << std::endl;

	CloseHandle(hThread);//closehandle()내가 스레드제어를 안하겠다는 함수, 강제종료아님!!!! // 스레드가 돌고있는데 닫으면 안닫힘!!
	closesocket(sock);
	WSACleanup();
	CloseHandle(g_hConnectEvent);
}