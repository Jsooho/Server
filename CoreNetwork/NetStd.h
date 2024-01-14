#pragma once
#include<WinSock2.h>
#include<iostream>
#include<windows.h>
#include<vector>
#include <list>


#pragma comment	(lib, "ws2_32.lib")
#ifdef _DEBUG
#pragma comment	(lib, "CoreNetwork_d.lib")
#else
#pragma comment	(lib, "CoreNetwork_r.lib")
#endif
using namespace std;

using namespace std;

