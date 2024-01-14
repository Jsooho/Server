#pragma once
#include "Server.h"
#define MAX_WORKER_THREAD 3

class LobbyServer : public Server
{
	//IOCP±¸Çö
public:
	HANDLE g_hWorkThread[MAX_WORKER_THREAD];
	HANDLE g_hIOCP;
public:
	bool AddUser(SOCKET sock, SOCKADDR_IN clientAddr);
public:
	virtual bool Init(int iport);
	virtual bool Run();
	virtual bool Release();
};

