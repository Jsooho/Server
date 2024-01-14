#pragma once
#include"HProtocol.h"
#include<string>
class HPacket
{
public:
	char*	m_pOffset;
	UPACKET m_uPacket;
public:
	void PutData(const char* pData, int iSize);
	void GetData(const char* pData, int iSize);
public:
	HPacket& operator << (int data);
	HPacket& operator << (long data);
	HPacket& operator << (short data);
	HPacket& operator << (float data);
	HPacket& operator << (char* data);
	HPacket& operator << (DWORD data);
	HPacket& operator << (std::string data);
	//HPacket& operator << (byte data);
public:
	HPacket& operator >> (int& data);
	HPacket& operator >> (long& data);
	HPacket& operator >> (short& data);
	HPacket& operator >> (float& data);
	HPacket& operator >> (char* data);
	HPacket& operator >> (DWORD& data);
	HPacket& operator >> (std::string& data);
	//HPacket& operator >> (byte& data);
public:
	HPacket();
	HPacket(WORD iType);
	HPacket(const HPacket& packet);
	~HPacket();
};

