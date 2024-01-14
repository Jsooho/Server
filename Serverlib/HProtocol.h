#pragma once
#include <windows.h>
// ��������: ��ǻ�� ���� ��� ����� �ϴ°��� ���� �������� ��Ģ���� ��Ƴ��� ���� ����
#define PACKET_HEADER_SIZE	 4
#define PACKET_MAX_DATA_SIZE 4096

#pragma pack(push, 1)
typedef struct {
	WORD len;
	WORD type;
}PACKET_HEADER;

typedef struct {
	PACKET_HEADER	ph;
	char			msg[PACKET_MAX_DATA_SIZE];
}UPACKET, *P_UPACKET;

struct ChatMsg
{
	char	name[20];
	char	message[256];
};

//�����
/*
ChatMsg cmsg;
cmsg.name = "��ȣ";
cmsg.message = "hiyo";
UPACKET Sendpacket
Sendpacket.ph.len =  strlen(cmsg) + PACKET_HEADER_SIZE;
Sendpacket.ph.type = PACKET_CHAT_MSG;
Sendpacket.msg = cmsg;
*/


#pragma pack(pop)

#define PACKET_CHAT_MSG      1000