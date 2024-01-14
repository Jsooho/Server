#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "LobbyServer.h"
#include "Accepter.h"


void main()
{
	LobbyServer server;
	server.Init(9190);
	Accepter accept(&server);
	accept.Detach();
	server.Run();
	server.Release();
}