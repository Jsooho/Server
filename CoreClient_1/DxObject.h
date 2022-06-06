#pragma once
#include "Std.h"
#include "HVector.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
struct SimpleVertex
{
	HVector2 v;
	HVector2 t;
};
class DxObject
{
public: //Texture
	ID3D11ShaderResourceView*	m_pSRV0;
	ID3D11Texture2D*			m_pTexture0;
	ID3D11ShaderResourceView*	m_pSRV1;
	ID3D11Texture2D*			m_pTexture1;
	ID3D11BlendState*			m_AlphaBlend;
	D3D11_TEXTURE2D_DESC		m_TextureDesc;
public:
	float		m_fSpeed;
	HVector2	m_vPos;
	float		m_fWidth;
	float		m_fHeight;
public:
	std::vector<SimpleVertex>	m_VertexList;
	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11InputLayout*			m_pVertexLayout;
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11PixelShader*			m_pPixelShader;
	ID3D11DeviceContext*		m_pContext;
	ID3DBlob*					m_pVSBlob = nullptr;
	ID3DBlob*					m_pErrorMsgs = nullptr;
	ID3DBlob*					m_pPSBlob = nullptr;
public:
	bool LoadShader();
	bool CreateVertexLayout();
	bool LoadTexture(std::wstring texturename, std::wstring maskname);
public:
	virtual bool  SetVertexData();
	void SetContext(ID3D11DeviceContext* m_pContext);
	bool Create(ID3D11DeviceContext* m_pContext);
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
public:
	DxObject();
	~DxObject();
};

