#include "HDxState.h"

ID3D11SamplerState* HDxState::g_pCurrentSS[3] = { nullptr,nullptr,nullptr };
ID3D11SamplerState* HDxState::g_pWrapSS = nullptr;
 ID3D11SamplerState* HDxState::g_pClampSS = nullptr;
HRESULT HDxState::CreateSamplerState()
{
	HRESULT hr = S_OK;
    D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = g_pd3dDevice->CreateSamplerState(&sd, &g_pWrapSS);
	if (FAILED(hr))
	{
		return hr;
	}
	
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	hr = g_pd3dDevice->CreateSamplerState(&sd, &g_pClampSS);

    return hr;
}

bool HDxState::Init()
{
	CreateSamplerState();
	return true;
}

bool HDxState::Release()
{
	if (g_pWrapSS) g_pWrapSS->Release();
	g_pWrapSS = nullptr;
	if (g_pClampSS) g_pClampSS->Release();
	g_pClampSS = nullptr;
	return true;
}
