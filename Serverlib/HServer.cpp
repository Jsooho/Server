#include "HServer.h"

int HServer::SendMsg(SOCKET sock, char* msg, WORD type)
{
    //패킷 생성
    UPACKET packet;
    ZeroMemory(&packet, sizeof(UPACKET));
    packet.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
    packet.ph.type = type;
    memcpy(packet.msg, msg, strlen(msg));

    //패킷 전송
    char* pMessage = (char*)&packet;
    int iSendsize = 0;



    return 0;
}

bool HServer::Init(int iport)
{
    SOCKADDR_IN m_Saddr;
    
    WSACreate();

    m_ListenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (m_ListenSock == SOCKET_ERROR)
    {
        cout << "Socket Error" << endl;
        return false;
    }
    ZeroMemory(&m_Saddr, sizeof(m_Saddr));

    m_Saddr.sin_family = AF_INET;
    m_Saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_Saddr.sin_port = htons(iport);

    if (bind(m_ListenSock, (SOCKADDR*)&m_Saddr, sizeof(m_Saddr)) == SOCKET_ERROR)
    {
        cout << "bind Error" << endl;
        return false;
    }

    if (listen(m_ListenSock, SOMAXCONN) == SOCKET_ERROR)
    {
        cout << "Listen Error" << endl;
        return false;
    }

    cout << "Server Start!" << endl;

    u_long on = 1;
    ioctlsocket(m_ListenSock, FIONBIO, &on);

    return true;
}

bool HServer::Run()
{
    return true;
}

bool HServer::Release()
{
    closesocket(m_ListenSock);
    WSACleanup();
    return true;
}

bool HServer::WSACreate()
{
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "WSA Error" << endl;
        return false;
    }
    return true;
}

