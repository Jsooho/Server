#include "HDevice.h"
ID3D11Device* g_pd3dDevice = nullptr;

HRESULT HDevice::InitDevice()
{
    HRESULT hr = S_OK;
    
    CreateDeviceAndSwapChain(m_hWnd,m_rtClient.right,m_rtClient.bottom);

    g_pd3dDevice = m_pd3dDevice;
    CreateRenderTargetView();
    SetViewport();
    return hr;
}

bool HDevice::CreateDeviceAndSwapChain(HWND hWnd, int Width, int Height )
{
    //D2DWrite 사용시 플래그값!!
    UINT Flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    D3D_FEATURE_LEVEL fl[]
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };
    
    ZeroMemory(&m_SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
    m_SwapChainDesc.BufferDesc.Width = Width;
    m_SwapChainDesc.BufferDesc.Height = Height;
    m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    m_SwapChainDesc.SampleDesc.Count = 1;
    m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    m_SwapChainDesc.BufferCount = 1;
    m_SwapChainDesc.OutputWindow = hWnd;
    m_SwapChainDesc.Windowed = true;


    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        Flags,
        fl,
        1,
        D3D11_SDK_VERSION,
        &m_SwapChainDesc,
        &m_pSwapchain,
        &m_pd3dDevice,
        &m_FeatureLevel,
        &m_pImmediateContext
    );
    
    if (FAILED(hr))
    {
        return false;
    }
      
    return true;
}
bool HDevice::CreateRenderTargetView()
{
    HRESULT hr = S_OK;
    ID3D11Texture2D* backBuffer = nullptr;
    m_pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    hr = m_pd3dDevice->CreateRenderTargetView(backBuffer, NULL, &m_pd3dRenderTargetView);
    if (FAILED(hr))
    {
        return false;
    }
    if (backBuffer)backBuffer->Release();

    m_pImmediateContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, NULL);

    return true;
}

bool HDevice::SetViewport()
{
    m_Viewport.TopLeftX = 0;
    m_Viewport.TopLeftY = 0;
    m_Viewport.Width    = m_SwapChainDesc.BufferDesc.Width;
    m_Viewport.Height   = m_SwapChainDesc.BufferDesc.Height;
    m_Viewport.MinDepth = 0.0f;
    m_Viewport.MaxDepth = 1.0f;
    m_pImmediateContext->RSSetViewports(1, &m_Viewport);
    return true;
}
bool HDevice::CleapupDevice()
{
    if (m_pd3dDevice)m_pd3dDevice->Release();	// 디바이스 객체
    if (m_pImmediateContext)m_pImmediateContext->Release();// 다비이스 컨텍스트 객체
    if (m_pSwapchain)m_pSwapchain->Release();	// 스왑체인 객체
    if (m_pd3dRenderTargetView)m_pd3dRenderTargetView->Release();
    m_pd3dDevice = nullptr;	// 디바이스 객체
    m_pImmediateContext = nullptr;// 다비이스 컨텍스트 객체
    m_pSwapchain = nullptr;	// 스왑체인 객체
    m_pd3dRenderTargetView = nullptr;
    return true;
}
HDevice::HDevice()
{
    m_pd3dDevice = nullptr;	// 디바이스 객체
    m_pImmediateContext = nullptr;// 다비이스 컨텍스트 객체
    m_pSwapchain = nullptr;	// 스왑체인 객체
    m_pd3dRenderTargetView = nullptr;
}
HDevice::~HDevice()
{}