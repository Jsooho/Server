#pragma once
#include "HCore.h"
#include "HVector.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
struct PS_VECTOR
{
	float3 pos;
	float2 texture;
};

class Sample : public HCore
{
public:
	vector<PS_VECTOR> m_Vertexlist;
	ID3D11Buffer* m_VertexBuffer;
	ID3D11InputLayout* m_VertexLayout;
	ID3DBlob* m_VSBlob;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;
public:
	ID3D11ShaderResourceView* m_pSRV0;
	ID3D11Resource* m_pTexture0;
	ID3D11ShaderResourceView* m_pSRV1;
	ID3D11Resource* m_pTexture1;
	ID3D11BlendState* m_AlphaBlend;
public:
	bool CreateVertexBuffer();
	bool LoadShader();
	bool CreateVertexLayout();
	bool LoadTexture();
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

