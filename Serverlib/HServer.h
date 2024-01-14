#pragma once
#include "HNetUser.h"
class HServer : public HServerObj
{
public:
	list<HNetUser*> m_Userlist;
	SOCKET			m_ListenSock;
public:
	int SendMsg(SOCKET sock, char* msg, WORD type);
	int SendMsg(SOCKET sock, UPACKET& packet);

public:
	virtual bool Init(int iport);
	virtual bool Run();
	virtual bool Release();
public:
	bool WSACreate();
};

