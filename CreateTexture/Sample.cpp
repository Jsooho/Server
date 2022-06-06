#include "Sample.h"

GAMECORE(asdad, 800, 600);

bool Sample::CreateVertexBuffer()
{
	PS_VECTOR vb[6];

    vb[0].pos.x = -0.5f; vb[0].pos.y = 0.5f; vb[0].pos.z = 1.0f; vb[0].texture.x = 0.0f; vb[0].texture.y = 0.0f;
	vb[1].pos.x = 0.5f; vb[1].pos.y = 0.5f; vb[1].pos.z = 1.0f; vb[1].texture.x = 1.0f; vb[1].texture.y = 0.0f;
	vb[2].pos.x = -0.5f; vb[2].pos.y = -0.5f; vb[2].pos.z = 1.0f; vb[2].texture.x = 0.0f; vb[2].texture.y = 1.0f;
	vb[3].pos.x = -0.5f; vb[3].pos.y = -0.5f; vb[3].pos.z = 1.0f; vb[3].texture.x = 0.0f; vb[3].texture.y = 1.0f;
	vb[4].pos.x = 0.5f; vb[4].pos.y = 0.5f; vb[4].pos.z = 1.0f; vb[4].texture.x = 1.0f; vb[4].texture.y = 0.0f;
	vb[5].pos.x = 0.5f; vb[5].pos.y = -0.5f; vb[5].pos.z = 1.0f; vb[5].texture.x = 1.0f; vb[5].texture.y = 1.0f;

	for (int i = 0; i < _countof(vb); i++)
	{
		m_Vertexlist.push_back(vb[i]);
	}

    HRESULT hr = S_OK;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(PS_VECTOR) * m_Vertexlist.size();
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA sd;
    ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
    sd.pSysMem = &m_Vertexlist.at(0);
    hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_VertexBuffer);

    if (FAILED(hr))
    {
        return false;
    }

    return true;
}
bool Sample::LoadTexture()
{
    HRESULT hr = S_OK;

    hr = DirectX::CreateWICTextureFromFile(m_pd3dDevice, L"../../data/bitmap1.bmp", &m_pTexture0, &m_pSRV0);
    if (FAILED(hr))
    {
        hr = DirectX::CreateDDSTextureFromFile(m_pd3dDevice, L"../../data/bitmap1.bmp", &m_pTexture0, &m_pSRV0);
    }
    hr = DirectX::CreateWICTextureFromFile(m_pd3dDevice, L"../../data/bitmap2.bmp", &m_pTexture1, &m_pSRV1);

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
    blend.RenderTarget[0].RenderTargetWriteMask =
        D3D11_COLOR_WRITE_ENABLE_ALL;
    m_pd3dDevice->CreateBlendState(&blend, &m_AlphaBlend);
    return true;
}
bool Sample::LoadShader()
{
    HRESULT hr = S_OK;
    ID3DBlob* error = nullptr;

    hr = D3DCompileFromFile(L"VertexShader.txt", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &m_VSBlob, nullptr);
    if (FAILED(hr))
    {
        return false;
    }
    hr = m_pd3dDevice->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), NULL, &m_pVS);
    if (FAILED(hr))
    {
        return false;
    }

    ID3DBlob* PSblob;
    hr = D3DCompileFromFile(L"PixelShader.txt", NULL, NULL, "PS", "ps_5_0", 0, 0, &PSblob, &error);
    if (FAILED(hr))
    {
        MessageBoxA(NULL,
            (char*)error->GetBufferPointer(),
            "error", MB_OK);
        if (error) error->Release();
        return false;
    }
    hr = m_pd3dDevice->CreatePixelShader(PSblob->GetBufferPointer(), PSblob->GetBufferSize(), NULL, &m_pPS);
    if (FAILED(hr))
    {
        return false;
    }

    PSblob->Release();

    return true;
}
bool Sample::CreateVertexLayout()
{
    HRESULT hr = S_OK;
    D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    int num = sizeof(vertexDesc) / sizeof(vertexDesc[0]);
    hr = m_pd3dDevice->CreateInputLayout(vertexDesc, num, m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), &m_VertexLayout);
    if (FAILED(hr))
    {
        return false;
    }
    return true;
}

bool Sample::Init()
{
    CreateVertexBuffer();
    LoadTexture();
    LoadShader();
    CreateVertexLayout();
    return true;
}
bool Sample::Frame()
{
    return true;
}
bool Sample::Render()
{
    m_pImmediateContext->PSSetShaderResources(0, 1, &m_pSRV0);
    m_pImmediateContext->PSSetShaderResources(1, 1, &m_pSRV1);
    m_pImmediateContext->OMSetBlendState(m_AlphaBlend, 0, -1);
    
    m_pImmediateContext->IASetInputLayout(m_VertexLayout);
    m_pImmediateContext->VSSetShader(m_pVS, NULL, 0);
    m_pImmediateContext->PSSetShader(m_pPS, NULL, 0);
    UINT pStrides = sizeof(PS_VECTOR);
    UINT pOffsets = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &pStrides, &pOffsets);
    m_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_pImmediateContext->Draw(m_Vertexlist.size(), 0);
    
    return true;
}
bool Sample::Release()
{
    if (m_AlphaBlend) m_AlphaBlend->Release();
    if (m_VertexBuffer) m_VertexBuffer->Release();
    if (m_VertexLayout) m_VertexLayout->Release();
    if (m_pVS) m_pVS->Release();
    if (m_pPS) m_pPS->Release();
    if (m_VSBlob) m_VSBlob->Release();
    if (m_pSRV0) m_pSRV0->Release();
    if (m_pTexture0) m_pSRV0->Release();
    if (m_pSRV1) m_pSRV1->Release();
    if (m_pTexture1) m_pTexture1->Release();
    m_pSRV0 = nullptr;
    m_pSRV1 = nullptr;
    m_pTexture0 = nullptr;
    m_pTexture1 = nullptr;
    m_AlphaBlend = nullptr;
    m_VertexBuffer = nullptr;
    m_VertexLayout = nullptr;
    m_pVS = nullptr;
    m_pPS = nullptr;
    m_VSBlob = nullptr;
    return true;
}