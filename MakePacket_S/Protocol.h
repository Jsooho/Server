#pragma once
#include <Windows.h>
// 패킷은 통신에서 하나의 데이터가 오고 가는 단위를 의미한다. 즉, 여러 개의 물건들을 목적지로 이동시키기 위해 포장을하는데 포장된 하나의 물건이다
#pragma pack(push, 1)

typedef struct
{
	WORD len;
	WORD type;
}PACKET_HEADER;

typedef struct
{
	PACKET_HEADER pheader;
	char          msg[4096];
}UPACKET, *P_UPACKET;

struct ChatMsg
{
	char name[20];
	char message[256];
};

#pragma pack(pop)

#define PACKET_HEADER_SIZE	 4
#define PACKET_CHAT_MSG      1000