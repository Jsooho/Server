#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "ServerObj.h"
#include "Packet.h"

struct OV 
{
	OVERLAPPED ov;
	int type;
};

class NetUser : public ServerObj
{
public:
	bool		m_bConnect = false; // 플래그
	SOCKET		m_Sock; 
	SOCKADDR_IN m_Addr;
	std::string m_csName;
	short       m_iPort;
	//WSARecv = WSABUF를 사용해야한다
	OV	   m_ovRead;
	OV	   m_ovSend;
	WSABUF m_wsaRecvBuf;
	WSABUF m_wsaSendBuf;
	char   m_szRecv[256];
	char   m_szSend[256];
public:
	char m_szRecvBuffer[2048];
	int  m_iPacketPos; // 패킷의 시작주소
	int  m_iWritePos;  // 현재의 저장 위치
	int  m_iReadPos;
	std::list<Packet> m_packetPool;
	int Dispatch(DWORD dwTrans, OV* ov);
	int DispatchRead(char* szRecvBuffer, int iRecvByte);
	int DispatchSend(DWORD transfer);
	void set(SOCKET sock, SOCKADDR_IN addr);
	int Recv();
	bool DisConnect();
public:
	NetUser();
	virtual ~NetUser();
};

