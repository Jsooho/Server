#pragma once
#include <winsock2.h>
#include <windows.h>
#include <WS2tcpip.h> //inet_ntop
#include <vector>
#include <list>
#include <iostream>
#pragma comment	(lib, "ws2_32.lib")
#ifdef _DEBUG
#pragma comment	(lib, "CoreServer_d.lib")
#else
#pragma comment	(lib, "CoreServer_r.lib")
#endif
using namespace std;