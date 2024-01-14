#pragma once
#include "Std.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
class HDxWrite
{
public:
	ID2D1Factory* m_pd2dFactory; // 디바이스와 같은 역할
	IDWriteFactory* m_pdWriteFactory;
	ID2D1RenderTarget* m_pd2dRenderTarger;
	IDWriteTextFormat* m_pTextFormat;
	ID2D1SolidColorBrush* m_pTextBrush;
public:
	bool CreateFactory();
	bool CreateDeviceResources(IDXGISurface1* pSurface);
	bool DrawText(RECT rt, std::wstring string, D2D1::ColorF color, IDWriteTextFormat* pTextFormat = nullptr);
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

