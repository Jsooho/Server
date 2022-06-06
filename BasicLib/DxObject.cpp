#include "DxObject.h"
#include "ObjManager.h"

bool DxObject::CreateVertexShader(wstring filename)
{
	m_VsShader = I_Shader.CreateVertexShader(filename, "VS");
	return true;
}
bool DxObject::CreatePixelShader(wstring filename)
{
	m_PsShader = I_Shader.CreatePixelShader(filename, "PS");
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
	 hr = g_pd3dDevice->CreateInputLayout(layout, NumElements, m_VsShader->m_pVSBlob->GetBufferPointer(), m_VsShader->m_pVSBlob->GetBufferSize(), &m_pVertexLayout);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool DxObject::LoadTexture(std::wstring texturename, std::wstring maskname)
{
	
	m_Texture = I_Texture.Load(texturename);
	
	if (!maskname.empty())
	{
		m_maskTexture = I_Texture.Load(maskname);
	}

	m_TextureDesc = m_Texture->m_TextureDesc;

	return true;
}

bool DxObject::CreateVertexBuffer()
{
	HRESULT hr = S_OK;
	if (m_VertexList.size() <= 0)
	{
		return false;
	}
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(PS_VECTOR) * m_VertexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_VertexList.at(0);

	if (FAILED(hr = g_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer)))
	{
		return false;
	}
	return true;
}

bool DxObject::CreateIndexBuffer()
{

	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_IndexList.at(0);
	hr = g_pd3dDevice->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	m_iNumIndex = m_IndexList.size();
	
	return true;
}

bool DxObject::CreateConstantBuffer()
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(CB_DATA);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &m_ConstantList;
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pConstantBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool DxObject::SetIndexData()
{
	return true;
}

bool DxObject::SetVertexData()
{
	return true;
}

bool DxObject::SetConstantData()
{
	ZeroMemory(&m_ConstantList, sizeof(CB_DATA));
	m_ConstantList.Color.x = 0.0f;
	m_ConstantList.Color.y = 1.0f;
	m_ConstantList.Color.z = 0.0f;
	m_ConstantList.Color.w = 1.0f;
	m_ConstantList.Timer.x = 0.0f;
	m_ConstantList.Timer.y = 1.0f;
	m_ConstantList.Timer.z = 0.0f;
	m_ConstantList.Timer.w = 0.0f;
	return true;
}

void DxObject::SetContext(ID3D11DeviceContext* pContext)
{
	m_pContext = pContext;
}

bool DxObject::Create(ID3D11DeviceContext* pContext, wstring Colorname, wstring maskname, wstring filename)
{
	HRESULT hr;

	m_rtCollision = Rect(m_vPos, m_fWidth, m_fHeight);

	I_ObjManager.AddCollisionExecute(this,
		std::bind(&BaseObject::HitOverlap, this,
			std::placeholders::_1,
			std::placeholders::_2));

	I_ObjManager.AddSelectExecute(this,
		std::bind(&BaseObject::SelectHitOverlap, this,
			std::placeholders::_1,
			std::placeholders::_2));
	
	SetContext(pContext);

	if (!LoadTexture(Colorname, maskname))
	{
		return false;
	}
	if (!SetVertexData())
	{
		return false;
	}
	if (!SetIndexData())
	{
		return false;
	}
	if (!SetConstantData())
	{
		return false;
	}
	if (!CreateVertexBuffer())
	{
		return false;
	}
	if (!CreateIndexBuffer())
	{
		return false;
	}
	if (!CreateConstantBuffer())
	{
		return false;
	}
	if (!CreateVertexShader(filename))
	{
		return false;
	}
	if (!CreatePixelShader(filename))
	{
		return false;
	}
	if (!CreateVertexLayout())
	{
		return false;
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
	hr = g_pd3dDevice->CreateBlendState(&blend, &m_AlphaBlend);

	blend.RenderTarget[0].BlendEnable = FALSE;
	hr = g_pd3dDevice->CreateBlendState(&blend, &m_AlphaBlendDisable);

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
	if (m_Texture != nullptr)
	{
		m_pContext->PSSetShaderResources(0, 1, &m_Texture->m_pSRV);
	}

	if (m_maskTexture != nullptr)
	{
		m_pContext->PSSetShaderResources(1, 1, &m_maskTexture->m_pSRV);
	}
	if (m_bAlphaBlend)
	{
		m_pContext->OMSetBlendState(m_AlphaBlend, 0, -1);
	}
	else
	{
		m_pContext->OMSetBlendState(m_AlphaBlendDisable, 0, -1);
	}

	if (m_VsShader != nullptr)
	{
		m_pContext->VSSetShader(m_VsShader->m_pVertexShader, NULL, 0);
	}
	if (m_PsShader != nullptr)
	{
		m_pContext->PSSetShader(m_PsShader->m_pPixelShader, NULL, 0);
	}
	if (m_pVertexLayout != nullptr)
	{
		m_pContext->IASetInputLayout(m_pVertexLayout);
	}
	UINT pStrides = sizeof(PS_VECTOR);
	UINT pOffsets = 0;
	m_pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &pStrides, &pOffsets);
	m_pContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	if (m_IndexList.size() < 0)
	{
		m_pContext->Draw(m_VertexList.size(), 0);
	}
	else
	{
		m_pContext->DrawIndexed(m_IndexList.size(), 0, 0);
	}
	return true;
}
bool DxObject::Release()
{
	
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pVertexLayout) m_pVertexLayout->Release();

	m_pVertexBuffer = nullptr;
	m_pVertexLayout = nullptr;
	
	if (m_AlphaBlend) m_AlphaBlend->Release();
	if (m_AlphaBlendDisable) m_AlphaBlendDisable->Release();
	m_AlphaBlend = nullptr;
	m_AlphaBlendDisable = nullptr;
	return true;
}
DxObject::DxObject()
{
	m_fSpeed = 0.0001f;
}
DxObject::~DxObject()
{}

void BaseObject::HitOverlap(BaseObject* pObj, DWORD dwState)
{
	
}

void BaseObject::SelectHitOverlap(BaseObject* pObj, DWORD dwState)
{
}
