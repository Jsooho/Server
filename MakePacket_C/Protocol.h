#pragma once
#include <Windows.h>

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