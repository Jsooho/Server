#include "NetUser.h"
int NetUser::Recv()
{
	//해당 소켓에 해당 버퍼를 리시브할수있게 만든다
	//배열당 패킷 한개인데 배열로 넘길수 있다.
	m_wsaRecvBuf.len = sizeof(char)*256;
	m_wsaRecvBuf.buf = m_szRecv;
	DWORD dwRead;
	DWORD dwFlag = 0;
	m_ovRead.type = 1000; //ket값 Recv
	//m_ovRead.type = 2000; // Send
	BOOL ret = WSARecv(m_Sock, &m_wsaRecvBuf, 1, &dwRead, &dwFlag, (WSAOVERLAPPED*)&m_ovRead, nullptr);
	return 0;
}
int NetUser::Dispatch(DWORD dwTrans, OV* ov)
{
	if (m_bConnect == false)
	{
		return 0;
	}
	if (ov->type == 1000)
	{
		if (!DispatchRead(m_szRecv, dwTrans))
		{

		}
		Recv();
	}
	/*if (ov->type == 2000)
	{
		if (!DispatchSend(dwTrans))
		{

		}
	}*/
	return 1;
}
int NetUser::DispatchRead(char* szRecvBuffer, int iRecvByte)
{
	//p1(m_iPacketPos)  p2(2.1)       pn   (m_ReadPos)
	//2035 ~ 2038 ~ 22  ~ 50  ~  2028 ~ 2038 ~ 2048 
	//0 ~ 5 ~ iRecvByte
	if (m_iWritePos + iRecvByte >= 2048)
	{
		if (m_iReadPos > 0)
		{
			memmove(&m_szRecvBuffer[0], &m_szRecvBuffer[m_iPacketPos], m_iReadPos);
		}
		m_iPacketPos = 0;
		m_iWritePos = m_iReadPos;
	}
	memcpy(&m_szRecvBuffer[m_iWritePos], szRecvBuffer, iRecvByte);
	m_iWritePos += iRecvByte;// 버퍼에 이전에 저장된 위치
	m_iReadPos += iRecvByte; // 패킷시작 위치로부터 받은 바이트

	if (m_iReadPos >= PACKET_HEADER_SIZE)
	{
		// 패킷 해석 가능
		UPACKET* pPacket = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
		// 적어도 1개의 패킷은 도착했다.
		if (pPacket->ph.len <= m_iReadPos)
		{
			do {
				Packet tPacket(pPacket->ph.type);
				memcpy(&tPacket.m_uPacket, &m_szRecvBuffer[m_iPacketPos], pPacket->ph.len);
				m_packetPool.push_back(tPacket);

				// 다음패킷 처리
				m_iPacketPos += pPacket->ph.len;
				m_iReadPos -= pPacket->ph.len;
				if (m_iReadPos < PACKET_HEADER_SIZE)
				{
					break;
				}
				pPacket = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
			} while (pPacket->ph.len <= m_iReadPos);
		}
	}
	return 1;
}

int NetUser::DispatchSend(DWORD transfer)
{
	return 0;
}

bool NetUser::DisConnect()
{
	closesocket(m_Sock);
	return true;
}

void NetUser::set(SOCKET sock, SOCKADDR_IN addr)
{
	m_bConnect = true;
	ZeroMemory(m_szRecvBuffer, sizeof(char) * 2048);
	m_iPacketPos = 0;
	m_iWritePos = 0;
	m_iReadPos = 0;
	int  m_iWritePos;  // 현재의 데이터 저장 위치
	int  m_iReadPos;   // 현재의 데이터 저장 위치

	m_Sock = sock;
	m_Addr = addr;
	m_csName = inet_ntoa(addr.sin_addr);
	m_iPort = ntohs(addr.sin_port);
}

NetUser::NetUser()
{

}
NetUser::~NetUser()
{

}