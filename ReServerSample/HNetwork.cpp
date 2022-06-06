#include "HNetwork.h"

bool HNetwork::Init(int iport)
{
    m_Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN saddr;

	ZeroMemory(&saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(iport);

	if (bind(m_Sock, (SOCKADDR*)&saddr, sizeof(saddr)) == SOCKET_ERROR)
	{
		cout << "bind error" << endl;
		return false;
	}

	if (listen(m_Sock, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "listen error" << endl;
		return false;
	}
	ThreadCreate();
	std::cout << "Server Start!" << std::endl;
	Sleep(1000);
	std::cout << "Server On..." << std::endl;

    return true;
}

bool HNetwork::Run()
{
	return true;
}

HNetwork::HNetwork()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
}
HNetwork::~HNetwork()
{

}