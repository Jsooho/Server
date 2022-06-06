#pragma once
#include "Hwindow.h"
class HDevice : public Hwindow
{
public:
	ID3D11Device*           m_pd3dDevice;
	IDXGISwapChain*         m_pSwapchain;
	ID3D11RenderTargetView* m_pd3dRenderTargetView;
	ID3D11DeviceContext*    m_pImmediateContext;

	D3D11_VIEWPORT          m_Viewport;
	D3D_FEATURE_LEVEL       m_FeatureLevel;
	DXGI_SWAP_CHAIN_DESC	m_SwapChainDesc;
public:
	bool CreateDeviceAndSwapChain(HWND hWnd, int Width, int Height);
	bool CreateRenderTargetView();
	bool SetViewport();
	bool CleapupDevice();
	HRESULT InitDevice();
public:
	HDevice();
	~HDevice();
};

