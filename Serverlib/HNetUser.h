#pragma once
#include"HServerObj.h"
#include"HPacket.h"
class HNetUser : public HServerObj
{
public:
	SOCKET		m_sock;
	SOCKADDR_IN m_addr;
	int			m_iport;
	string		m_sname;
public:
	void Set(SOCKET sock, SOCKADDR_IN addr);
};

