#pragma once
#include <windows.h>
// 프로토콜: 컴퓨터 간에 어떻게 통신을 하는가에 대한 여러가지 규칙들을 모아놓은 것을 말함
#pragma pack(push, 1)
typedef struct {
	WORD len;
	WORD type;
}PACKET_HEADER;

typedef struct {
	PACKET_HEADER	ph;
	char			msg[4096];
}UPACKET, *P_UPACKET;

struct ChatMsg
{	
	char	name[20]; 
	char	message[256];	
};
#pragma pack(pop)

#define PACKET_HEADER_SIZE	 4
#define PACKET_CHAT_MSG      1000 //type
