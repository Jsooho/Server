#pragma once
#include <windows.h>
#define PACKET_HEADER_SIZE 4

#pragma pack(push, 1)
typedef struct {
	WORD len;
	WORD type;
}PACKET_HEADER;

typedef struct {
	PACKET_HEADER	ph;
	char			msg[4096];
}UPACKET, * P_UPACKET;

struct TChatMsg
{
	char	name[20]; // ȫ�浿
	char	message[256];// �ȳ��ϼ���.
};
#pragma pack(pop)

#define PACKET_CHAT_MSG      1000
/*
* msg = "�ȳ�";
* UPACKET sendPacket;
* sendPacket.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
* sendPacket.ph.type = PACKET_CHAT_MSG;
* sendPacket.msg = msg;
*/

/*
* TChatMsg msg;
* msg.name = "ȫ�浿";
* msg.message = "�ȳ��ϼ���.";
* UPACKET sendPacket;
* sendPacket.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
* sendPacket.ph.type = PACKET_CHAT_MSG;
* sendPacket.msg = msg;
*/