#pragma once
#include "Protocol.h"
#include <string>

// Packet data;
// data << header << index << name << damage << mesage;
// data >> header >> index >> name >> damage >> mesage;
class Packet
{
public:
	char*	m_pOffset; //������
	char* c_str() { return m_pOffset; }
	UPACKET m_uPacket;
	void PutData(const char* pData, int iSize);
	void GetData(const char* pData, int iSize);
public:
	Packet& operator << (int data);
	Packet& operator << (long data);
	Packet& operator << (short data);
	Packet& operator << (float data);
	Packet& operator << (char* data);
	Packet& operator << (std::string data);
public:
	Packet& operator >> (int& data);
	Packet& operator >> (long& data);
	Packet& operator >> (short& data);
	Packet& operator >> (float& data);
	Packet& operator >> (char* data);
	Packet& operator >> (std::string& data);
public:
	Packet();
	Packet(WORD iType);
	Packet(const Packet& packet);
	~Packet();
};

