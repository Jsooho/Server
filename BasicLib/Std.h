#pragma once
#include<WinSock2.h>
#include<iostream>
#include<stdio.h>
#include <tchar.h>
#include<windows.h>
#include<vector>
#include<map>
#include <list>
#include <codecvt>
#include <atlconv.h>
#include <functional>
#include <d3d11.h>
#include<dxgi.h>
#include <d3dcompiler.h>
#include "Collision.h"

#pragma comment	(lib, "ws2_32.lib")
#ifdef _DEBUG
#pragma comment	(lib, "BasisLib_d.lib")
#else
#pragma comment	(lib, "BasisLib_r.lib")
#endif
#pragma comment	(lib, "d3d11.lib")
#pragma comment (lib,"dxgi.lib")
#pragma comment	(lib, "d3dcompiler.lib")
#pragma comment	(lib, "DirectXTK.lib")

using namespace std;
extern RECT g_rtClient;
extern HWND g_hWnd;
extern ID3D11Device* g_pd3dDevice;
extern float  g_fSecPerFrame;
extern float  g_fGameTimer;
extern POINT  g_ptMouse;


template<class T>
class Singleton
{
public:
	static T& Get()
	{
		static T theSingle;
		return theSingle;
	}
};

#define GAME_START int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
#define GAME_RUN(WindowName,x,y)  { Sample g_Sample; g_Sample.InitWindows(hInstance,nCmdShow,L#WindowName, x, y);g_Sample.WinRun(); }
#define GAMECORE(WindowName,x,y) GAME_START GAME_RUN(WindowName,x,y)


static std::wstring to_mw(const std::string& _src)
{
	USES_CONVERSION;
	return std::wstring(A2W(_src.c_str()));
};

static std::string to_wm(const std::wstring& _src)
{
	USES_CONVERSION;
	return std::string(W2A(_src.c_str()));
};
