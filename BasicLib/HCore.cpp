#include "HCore.h"
#include "ObjManager.h"
bool HCore::Init()
{
	
    return true;
}
bool HCore::Frame()
{
	
    return true;
}
bool HCore::Render()
{
    return true;
}
bool HCore::Release()
{
    return true;
}
bool  HCore::GameInit()
{
	InitDevice();
	HDxState::Init();
	m_Timer.Init();
	HInput::Get().Init();
	m_DxWrite.Init();
	
	IDXGISurface1* m_pBackBuffer;
	HRESULT hr = m_pSwapchain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&m_pBackBuffer);
	if (SUCCEEDED(hr))
	{
		m_DxWrite.CreateDeviceResources(m_pBackBuffer);
	}
	if (m_pBackBuffer)m_pBackBuffer->Release();

	Init();
	return true;
}
bool	HCore::GameRun()
{
	
	if (!GameFrame()) return false;
	if (!GameRender()) return false;
	return true;
}
bool	HCore::GameFrame()
{
	m_Timer.Frame();
	HInput::Get().Frame();
	I_ObjManager.Get().Frame();
	m_DxWrite.Frame();
	if (HInput::Get().GetKey('1') == KEY_PUSH)
	{
		m_bDebugText = !m_bDebugText;
	}
	Frame();
	return true;
}
bool	HCore::GameRender()
{
	float color[4] = { 1.0f, 1.0f, 1.0f,1.0f };
	//float color[4] = { 1, 1, 1,1.0f };
	m_pImmediateContext->ClearRenderTargetView(m_pd3dRenderTargetView, color);
	ApplySS(m_pImmediateContext, HDxState::g_pWrapSS, 0);
	//ApplySS(m_pImmediateContext, HDxState::g_pClampSS, 0);
	// 백버퍼에 랜더링 한다.
	m_Timer.Render();
	HInput::Get().Render();
	m_DxWrite.Render();
	Render();
	if (m_bDebugText)
	{
		RECT rt = { 0,0,400,300 };

		m_DxWrite.DrawText(rt, m_Timer.m_szTimerString, D2D1::ColorF(1, 0, 0, 1));
	}
	
	m_pSwapchain->Present(0, 0);
	return true;
}

bool	HCore::GameRelease()
{
	m_Timer.Release();
	HInput::Get().Release();
	m_DxWrite.Release();
	HDxState::Release();
	Release();
	CleapupDevice();
	return true;
}