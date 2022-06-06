#pragma once
#include"HCore.h"
#include"HVector.h"
struct PS_VECTOR
{
	HVector3 pos;
	HVector4 color;
};
class Sample :public HCore
{
	public:
		vector<PS_VECTOR> m_Vertexlist;
		ID3D11Buffer* m_VertexBuffer;
		ID3D11InputLayout* m_VertexLayout;
		ID3DBlob* m_VSBlob;
		ID3DBlob* m_PSblob;
		ID3D11VertexShader* m_pVS;
		ID3D11PixelShader* m_pPS;
	public:
		HRESULT CreateVertexBuffer();
		HRESULT LoadShader();
		HRESULT CreateVertexLayout();
	public:
		virtual bool Init();
		virtual bool Frame();
		virtual bool Render();
		virtual bool Release();
	
};

