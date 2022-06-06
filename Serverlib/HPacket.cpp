#include "HPacket.h"
void HPacket::PutData(const char* pData, int iSize)
{
	m_uPacket.ph.len += iSize;
	memcpy(m_pOffset, pData, iSize);
	m_pOffset += iSize;
};
void HPacket::GetData(const char* pData, int iSize)
{
	memcpy(const_cast<char*>(pData), m_pOffset, iSize);
	m_pOffset += iSize;
};

HPacket& HPacket::operator << (int data)
{
	PutData((char*)&data, sizeof(int));
	return *this;
}
HPacket& HPacket::operator << (long data)
{
	PutData((char*)&data, sizeof(long));
	return *this;
}
HPacket& HPacket::operator << (short data)
{
	PutData((char*)&data, sizeof(short));
	return *this;
}
HPacket& HPacket::operator << (float data)
{
	PutData((char*)&data, sizeof(float));
	return *this;
}
HPacket& HPacket::operator << (char* data)
{
	PutData(data, strlen(data) + 1);
	return *this;
}
HPacket& HPacket::operator << (std::string data)
{
	PutData(data.c_str(), data.size() + 1);
	return *this;
}
HPacket& HPacket::operator << (DWORD data)
{
	PutData((char*)&data, sizeof(DWORD));
	return *this;
}



HPacket& HPacket::operator >> (int& data)
{
	GetData((char*)&data, sizeof(int));
	return *this;
}
HPacket& HPacket::operator >> (long& data)
{
	GetData((char*)&data, sizeof(long));
	return *this;
}
HPacket& HPacket::operator >> (short& data)
{
	GetData((char*)&data, sizeof(short));
	return *this;
}
HPacket& HPacket::operator >> (float& data)
{
	GetData((char*)&data, sizeof(float));
	return *this;
}
HPacket& HPacket::operator >> (char* data)
{
	int iSize = strlen(m_pOffset) + 1;
	GetData(data, iSize);
	return *this;
}
HPacket& HPacket::operator >> (std::string& data)
{
	int iSize = strlen(m_pOffset + 1);
	GetData(data.c_str(), iSize);
	return *this;
}
HPacket& HPacket::operator >> (DWORD& data)
{
	GetData((char*)&data, sizeof(DWORD));
	return *this;
}
HPacket::HPacket()
{
	ZeroMemory(&m_uPacket, sizeof(UPACKET));
	m_uPacket.ph.len = PACKET_HEADER_SIZE;
	m_pOffset = m_uPacket.msg;
}
HPacket::HPacket(WORD iType)
{
	ZeroMemory(&m_uPacket, sizeof(UPACKET));
	m_uPacket.ph.len = PACKET_HEADER_SIZE;
	m_uPacket.ph.type = iType;
	m_pOffset = m_uPacket.msg;
}
HPacket::HPacket(const HPacket& packet)
{
	m_uPacket.ph = packet.m_uPacket.ph;
	memcpy(m_uPacket.msg, packet.m_uPacket.msg, PACKET_MAX_DATA_SIZE);
	m_pOffset = m_uPacket.msg;
}
HPacket::~HPacket()
{

}