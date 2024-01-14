#include "HNetUser.h"

void HNetUser::Set(SOCKET sock, SOCKADDR_IN addr)
{
	m_sock = sock;
	m_addr = addr;
	m_sname = inet_ntoa(addr.sin_addr);
	m_iport = ntohs(addr.sin_port);
}
