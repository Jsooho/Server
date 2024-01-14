#include "DxObject.h"

bool DxObject::LoadShader()
{
	HRESULT hr = S_OK;
	ID3DBlob* error = nullptr;

	hr = D3DCompileFromFile(L"VShader.txt", NULL, NULL, "VS", "vs_5_0", 0, 0, &m_pVSBlob, &m_pErrorMsgs);
	if (FAILED(hr))
	{
		MessageBoxA(NULL,
			(char*)m_pErrorMsgs->GetBufferPointer(),
			"ERROR", MB_OK);
		if (m_pErrorMsgs) m_pErrorMsgs->Release();
		return false;
	}
	hr = g_pd3dDevice->CreateVertexShader(m_pVSBlob->GetBufferPointer(),m_pVSBlob->GetBufferSize(),NULL,&m_pVertexShader);
	if (FAILED(hr))
	{
		return false;
	}

	hr = D3DCompileFromFile(L"PShader.txt",NULL,NULL,"PS","ps_5_0",0,0,&m_pPSBlob,&m_pErrorMsgs);
	if (FAILED(hr))
	{
		MessageBoxA(NULL,
			(char*)m_pErrorMsgs->GetBufferPointer(),
			"ERROR", MB_OK);
		if (m_pErrorMsgs) m_pErrorMsgs->Release();
		return false;
	}
	hr = g_pd3dDevice->CreatePixelShader(m_pPSBlob->GetBufferPointer(),m_pPSBlob->GetBufferSize(),NULL,&m_pPixelShader);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool DxObject::CreateVertexLayout()
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",0, DXGI_FORMAT_R32G32_FLOAT, 0,0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0,8,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT NumElements = sizeof(layout) / sizeof(layout[0]);
	 hr = g_pd3dDevice->CreateInputLayout(layout, NumElements, m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), &m_pVertexLayout);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool DxObject::LoadTexture(std::wstring texturename, std::wstring maskname)
{
	HRESULT hr = S_OK;

	hr = DirectX::CreateWICTextureFromFile(g_pd3dDevice, texturename.c_str(), (ID3D11Resource**)&m_pTexture0, &m_pSRV0);
	if (FAILED(hr))
	{
		hr = DirectX::CreateDDSTextureFromFile(g_pd3dDevice, texturename.c_str(), (ID3D11Resource**)&m_pTexture0, &m_pSRV0);
	}

	m_pTexture0->GetDesc(&m_TextureDesc);

	hr = DirectX::CreateWICTextureFromFile(g_pd3dDevice, maskname.c_str(), (ID3D11Resource**)&m_pTexture1, &m_pSRV1);
	if (FAILED(hr))
	{
		hr = DirectX::CreateDDSTextureFromFile(g_pd3dDevice, maskname.c_str(), (ID3D11Resource**)&m_pTexture1, &m_pSRV1);
	}

	D3D11_BLEND_DESC  blend;
	ZeroMemory(&blend, sizeof(D3D11_BLEND_DESC));
	/*bd.AlphaToCoverageEnable;
	bd.IndependentBlendEnable;*/
	blend.RenderTarget[0].BlendEnable = TRUE;
	blend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//// A 연산 저장
	blend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_pd3dDevice->CreateBlendState(&blend, &m_AlphaBlend);

	return true;
}

bool DxObject::SetVertexData()
{
	return true;
}

void DxObject::SetContext(ID3D11DeviceContext* pContext)
{
	m_pContext = pContext;
}

bool DxObject::Create(ID3D11DeviceContext* pContext)
{
	HRESULT hr;
	SetContext(pContext);
	if (!LoadTexture(L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp"))
	{
		return false;
	}
	if (!SetVertexData())
	{
		return false;
	}
	
	//gpu메모리에 버퍼 할당(원하는 할당 크기)
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(SimpleVertex) * m_VertexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_VertexList.at(0);

	if (FAILED(hr = g_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer)))
	{
		return false;
	}
	if (!LoadShader())
	{
		return false;
	}
	if (!CreateVertexLayout())
	{
		return false;
	}

	return true;
}
bool DxObject::Init()
{
	return true;
}
bool DxObject::Frame()
{
	return true;
}
bool DxObject::Render()
{
	m_pContext->PSSetShaderResources(0, 1, &m_pSRV0);
	m_pContext->PSSetShaderResources(1, 1, &m_pSRV1);
	m_pContext->OMSetBlendState(m_AlphaBlend, 0, -1);

	m_pContext->IASetInputLayout(m_pVertexLayout);
	m_pContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pContext->PSSetShader(m_pPixelShader, NULL, 0);
	UINT pStrides = sizeof(SimpleVertex);
	UINT pOffsets = 0;
	m_pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &pStrides, &pOffsets);
	m_pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pContext->Draw(m_VertexList.size(), 0);

	return true;
}
bool DxObject::Release()
{
	if (m_pVSBlob) m_pVSBlob->Release();
	if (m_pPSBlob) m_pPSBlob->Release();
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pVertexLayout) m_pVertexLayout->Release();
	if (m_pVertexShader) m_pVertexShader->Release();
	if (m_pPixelShader) m_pPixelShader->Release();
	m_pVertexBuffer = nullptr;
	m_pVertexLayout = nullptr;
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	m_pVSBlob = nullptr;
	m_pPSBlob = nullptr;
	return true;
}
DxObject::DxObject()
{
	m_fSpeed = 0.0001f;
}
DxObject::~DxObject()
{}
