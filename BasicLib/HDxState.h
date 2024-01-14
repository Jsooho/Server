#pragma once
#include "Std.h"
class HDxState
{
public:
	static ID3D11SamplerState* g_pCurrentSS[3];
	static ID3D11SamplerState* g_pWrapSS;
	static ID3D11SamplerState* g_pClampSS;
public:
	static HRESULT CreateSamplerState();
public:
	static bool Init();
	static bool Release();
public:
	HDxState() {};
	~HDxState() {};
};

static ID3D11SamplerState* ApplySS(ID3D11DeviceContext* pContext, ID3D11SamplerState* pState, UINT StartSlot = 0)
{
	ID3D11SamplerState* pSS = HDxState::g_pCurrentSS[StartSlot];
	pContext->PSSetSamplers(StartSlot, 1, &pState);
	HDxState::g_pCurrentSS[StartSlot] = pState;
	return pSS;
}
