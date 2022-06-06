#pragma once
#include "NetUser.h"
#include "ServerObj.h"
class Server : public ServerObj
{
public:
	SOCKET			 m_listenSock;
	list<NetUser*>	 m_Userlist;
	
public:
	int SendMsg(SOCKET sock, char* msg, WORD type);
	int SendMsg(SOCKET sock, UPACKET& packet);
	int Broadcast(NetUser* user);
	virtual bool AddUser(SOCKET sock, SOCKADDR_IN clientAddr);
public:
	virtual bool Init(int iport);
	virtual bool Run();
	virtual bool Release();
};

